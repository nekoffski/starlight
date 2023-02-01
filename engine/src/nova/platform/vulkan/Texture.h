#pragma once

#include <string>
#include <vector>

#include <stb.h>

#include <kc/core/Log.h>

#include "nova/core/Memory.hpp"
#include "nova/core/Id.h"
#include "nova/gfx/Texture.h"

#include "Vulkan.h"
#include "Image.h"
#include "Buffer.h"
#include "Context.h"
#include "Device.h"
#include "CommandBuffer.h"

namespace nova::platform::vulkan {

class Texture : public gfx::Texture {
   public:
    explicit Texture(
        const Context* context, Device* device, const std::string& name, uint32_t width,
        uint32_t height, uint8_t channels, const void* pixels, bool transparent
    )
        : m_context(context), m_device(device) {
        create(name, width, height, channels, pixels, transparent);
    }

    ~Texture() {
        const auto logicalDevice = m_device->getLogicalDevice();

        vkDeviceWaitIdle(logicalDevice);
        vkDestroySampler(logicalDevice, m_sampler, m_context->getAllocator());
    }

    Image* getImage() { return m_image.get(); }

    VkSampler getSampler() { return m_sampler; }

   protected:
    explicit Texture(const Context* context, Device* device)
        : m_context(context), m_device(device) {}

    void create(
        const std::string& name, uint32_t width, uint32_t height, uint8_t channels,
        const void* pixels, bool transparent
    ) {
        props.width      = width;
        props.height     = height;
        props.channels   = channels;
        this->generation = core::invalidId;

        VkDeviceSize imageSize = width * height * channels;
        VkFormat format        = VK_FORMAT_R8G8B8A8_UNORM;

        VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkMemoryPropertyFlags memoryProps =
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        Buffer staging(
            m_context, m_device,
            Buffer::Properties{
                .size                = imageSize,
                .memoryPropertyFlags = memoryProps,
                .usageFlags          = usage,
                .bindOnCreate        = true,
            }
        );
        staging.loadData(0, imageSize, 0, pixels);

        m_image.emplace(
            m_device, m_context,
            Image::Properties{
                VK_IMAGE_TYPE_2D,
                {width, height},
                format,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                    VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                true,
                VK_IMAGE_ASPECT_COLOR_BIT
        }
        );

        LOG_INFO("Here imageSize = {}, channels={}", imageSize, channels);

        CommandBuffer tempCommandBuffer{m_device, m_device->getGraphicsCommandPool()};
        VkQueue graphicsQueue = m_device->getQueues().graphics;

        tempCommandBuffer.createAndBeginSingleUse();

        LOG_INFO("Here 1");

        m_image->transitionLayout(
            tempCommandBuffer, format, VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        );

        LOG_INFO("Here 2");

        m_image->copyFromBuffer(staging, tempCommandBuffer);

        LOG_INFO("Here 3");

        m_image->transitionLayout(
            tempCommandBuffer, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

        LOG_INFO("Here 4");

        tempCommandBuffer.endSingleUse(graphicsQueue);

        // Create a sampler for the texture
        VkSamplerCreateInfo sampler_info = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
        // TODO: These filters should be configurable.
        sampler_info.magFilter               = VK_FILTER_LINEAR;
        sampler_info.minFilter               = VK_FILTER_LINEAR;
        sampler_info.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        sampler_info.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        sampler_info.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        sampler_info.anisotropyEnable        = VK_TRUE;
        sampler_info.maxAnisotropy           = 16;
        sampler_info.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        sampler_info.unnormalizedCoordinates = VK_FALSE;
        sampler_info.compareEnable           = VK_FALSE;
        sampler_info.compareOp               = VK_COMPARE_OP_ALWAYS;
        sampler_info.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        sampler_info.mipLodBias              = 0.0f;
        sampler_info.minLod                  = 0.0f;
        sampler_info.maxLod                  = 0.0f;

        VK_ASSERT(vkCreateSampler(
            m_device->getLogicalDevice(), &sampler_info, m_context->getAllocator(), &m_sampler
        ));

        props.isTransparent = transparent;
        generation++;
    }

   private:
    const Context* m_context;
    Device* m_device;

    core::LocalPtr<Image> m_image;
    VkSampler m_sampler;
};

class ImageTexture : public Texture {
   public:
    explicit ImageTexture(
        const Context* context, Device* device, const std::string& name, const std::string& path
    )
        : Texture(context, device) {
        int width;
        int height;
        int channels;

        constexpr int requiredChannels = 4;

        stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, requiredChannels);
        ASSERT(pixels, "Could not load image {}", path);

        create(name, width, height, requiredChannels, pixels, true);
        stbi_image_free(pixels);
    }
};

class TextureLoader : public gfx::TextureLoader {
   public:
    explicit TextureLoader(const Context* context, Device* device)
        : m_context(context), m_device(device) {}

    Texture* load(const std::string& name, const Texture::Properties& props, const void* pixels)
        const override {
        m_textures.emplace_back(core::createUniqPtr<Texture>(
            m_context, m_device, name, props.width, props.height, props.channels, pixels,
            props.isTransparent
        ));

        return m_textures.back().get();
    }

    Texture* load(const std::string& name, const std::string& path) const override {
        m_textures.emplace_back(core::createUniqPtr<ImageTexture>(m_context, m_device, name, path));

        return m_textures.back().get();
    }

   private:
    // TODO: use other data structure to avoid invalidating pointers and get rid of uniq_ptr
    mutable std::vector<core::UniqPtr<Texture>> m_textures;

    const Context* m_context;
    Device* m_device;
};

}  // namespace nova::platform::vulkan
