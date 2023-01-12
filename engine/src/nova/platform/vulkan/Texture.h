#pragma once

#include <string>

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
        const Context* context, Device* device, const std::string& name, bool autoRelease,
        uint32_t width, uint32_t height, uint8_t channels, const void* pixels, bool transparent
    )
        : m_context(context), m_device(device) {
        this->width      = width;
        this->height     = height;
        this->channels   = channels;
        this->generation = core::invalidId;

        VkDeviceSize imageSize   = width * height * channels;
        VkFormat format          = VK_FORMAT_R8G8B8A8_UNORM;
        VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkMemoryPropertyFlags memoryProps =
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        Buffer staging(
            context, device,
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

        static CommandBuffer tempCommandBuffer{m_device, m_device->getGraphicsCommandPool()};
        VkQueue graphicsQueue = m_device->getQueues().graphics;

        tempCommandBuffer.createAndBeginSingleUse();

        LOG_INFO("here2");
        m_image->transitionLayout(
            tempCommandBuffer, format, VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        );
        LOG_INFO("here");

        m_image->copyFromBuffer(staging, tempCommandBuffer);

        m_image->transitionLayout(
            tempCommandBuffer, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

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

        isTransparent = transparent;
        generation++;
        LOG_INFO("here3");
    }

    ~Texture() {
        const auto logicalDevice = m_device->getLogicalDevice();

        vkDeviceWaitIdle(logicalDevice);
        vkDestroySampler(logicalDevice, m_sampler, m_context->getAllocator());
    }

    Image* getImage() { return m_image.get(); }

    VkSampler getSampler() { return m_sampler; }

   private:
    const Context* m_context;
    Device* m_device;

    core::LocalPtr<Image> m_image;
    VkSampler m_sampler;
};

}  // namespace nova::platform::vulkan
