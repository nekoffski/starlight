#include "VKTexture.h"

#include <string>
#include <vector>

#include <stb.h>

#include <kc/core/Log.h>
#include <kc/core/Scope.hpp>

#include "Vulkan.h"
#include "VKImage.h"
#include "VKBuffer.h"
#include "VKContext.h"
#include "VKDevice.h"
#include "VKCommandBuffer.h"

namespace sl::vk {

static VkFormat channelsToFormat(u32 channels) {
    switch (channels) {
        case 1:
            return VK_FORMAT_R8_UNORM;
        case 2:
            return VK_FORMAT_R8G8_UNORM;
        case 3:
            return VK_FORMAT_R8G8B8_UNORM;
        case 4:
            return VK_FORMAT_R8G8B8A8_UNORM;
        default:
            return VK_FORMAT_R8G8B8A8_UNORM;
    }
}

VKImage::Properties getImageProperties(
  u32 width, u32 height, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM
) {
    return VKImage::Properties{
        VK_IMAGE_TYPE_2D,
        width,
        height,
        format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT
          | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        true,
        VK_IMAGE_ASPECT_COLOR_BIT
    };
}

VKTexture::VKTexture(
  u32 id, const VKContext* context, VKDevice* device, const Properties& props,
  const std::span<u8> pixels
) :
    Texture(props, id),
    m_context(context), m_device(device),
    m_image(
      m_device, m_context,
      getImageProperties(
        props.width, props.height, channelsToFormat(props.channels)
      ),
      pixels
    ),
    m_generation(1u) {
    LOG_TRACE("Texture created: {}", m_props.name);
}

VKTexture::VKTexture(
  u32 id, const VKContext* context, VKDevice* device, const Properties& props,
  VkImage handle, VkFormat format
) :
    Texture(props, id),
    m_context(context), m_device(device),
    m_image(
      device, context, getImageProperties(props.width, props.height, format), handle
    ),
    m_generation(1u) {
    LOG_TRACE("Texture created: {} from existing VKImage", m_props.name);
}

VKTexture::~VKTexture() { LOG_TRACE("Texture destroyed: {}", m_props.name); }

VKImage* VKTexture::getImage() { return &m_image; }

void VKTexture::resize(u32 width, u32 height) {
    m_props.width  = width;
    m_props.height = height;

    m_image.recreate(
      getImageProperties(width, height, channelsToFormat(m_props.channels))
    );
}

void VKTexture::resize(u32 width, u32 height, VkImage handle) {
    m_props.width  = width;
    m_props.height = height;

    auto props       = m_image.getProperties();
    props.width      = width;
    props.height     = height;
    props.createView = true;

    m_image.recreate(props, handle);
}

void VKTexture::write(u32 offset, std::span<u8> pixels) {
    m_image.write(offset, pixels);
}

static VkSamplerCreateInfo createSamplerCreateInfo(
  const TextureMap::Properties& props
) {
    // clang-format off
    static std::unordered_map<TextureMap::Repeat, VkSamplerAddressMode> vkRepeat{
        {TextureMap::Repeat::repeat,          VK_SAMPLER_ADDRESS_MODE_REPEAT},
        { TextureMap::Repeat::mirroredRepeat, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT},
        { TextureMap::Repeat::clampToEdge,    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE},
        { TextureMap::Repeat::clampToBorder,  VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER},
    };
    // clang-format on
    static std::unordered_map<TextureMap::Filter, VkFilter> vkFilter{
        {TextureMap::Filter::nearest, VK_FILTER_NEAREST},
        { TextureMap::Filter::linear, VK_FILTER_LINEAR }
    };

    VkSamplerCreateInfo samplerInfo     = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
    samplerInfo.magFilter               = vkFilter[props.magnifyFilter];
    samplerInfo.minFilter               = vkFilter[props.minifyFilter];
    samplerInfo.addressModeU            = vkRepeat[props.uRepeat];
    samplerInfo.addressModeV            = vkRepeat[props.vRepeat];
    samplerInfo.addressModeW            = vkRepeat[props.wRepeat];
    samplerInfo.anisotropyEnable        = VK_TRUE;
    samplerInfo.maxAnisotropy           = 16;
    samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable           = VK_FALSE;
    samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias              = 0.0f;
    samplerInfo.minLod                  = 0.0f;
    samplerInfo.maxLod                  = 0.0f;

    return samplerInfo;
}

VKTextureMap::VKTextureMap(
  u32 id, VKContext& context, VKDevice& device, const Properties& props,
  VKTexture& texture
) :
    TextureMap(props, id),
    m_context(context), m_device(device), m_texture(texture) {
    const auto samplerInfo = createSamplerCreateInfo(props);
    VK_ASSERT(vkCreateSampler(
      m_device.getLogicalDevice(), &samplerInfo, m_context.getAllocator(), &m_sampler
    ));
}

VKTextureMap::~VKTextureMap() {
    const auto logicalDevice = m_device.getLogicalDevice();

    vkDeviceWaitIdle(logicalDevice);
    vkDestroySampler(logicalDevice, m_sampler, m_context.getAllocator());
    LOG_TRACE("Texture map destroyed");
}

Texture* VKTextureMap::getTexture() const { return &m_texture; }

const VKImage* VKTextureMap::getImage() const { return m_texture.getImage(); }

VkSampler VKTextureMap::getSampler() const { return m_sampler; }

}  // namespace sl::vk
