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

static VkSamplerCreateInfo createSamplerCreateInfo() {
    VkSamplerCreateInfo samplerInfo = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
    // TODO: These filters should be configurable.
    samplerInfo.magFilter               = VK_FILTER_LINEAR;
    samplerInfo.minFilter               = VK_FILTER_LINEAR;
    samplerInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
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

VKTexture::VKTexture(
  const VKContext* context, VKDevice* device, const Properties& props, u32 id,
  const void* pixels
) :
    Texture(props, id),
    m_context(context), m_device(device), m_generation(0u) {
    LOG_TRACE("Creating Texture");
    VkDeviceSize imageSize = props.width * props.height * props.channels;
    VkFormat format        = VK_FORMAT_R8G8B8A8_UNORM;

    VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    VkMemoryPropertyFlags memoryProps =
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    VKBuffer::Properties stagingBufferProperties{
        .size                = imageSize,
        .memoryPropertyFlags = memoryProps,
        .usageFlags          = usage,
        .bindOnCreate        = true,
        .useFreeList         = false
    };

    VKBuffer stagingBuffer(m_context, m_device, stagingBufferProperties);

    stagingBuffer.loadData(0, imageSize, 0, pixels);

    VKImage::Properties imageProperties{
        VK_IMAGE_TYPE_2D,
        {props.width, props.height},
        format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT
          | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        true,
        VK_IMAGE_ASPECT_COLOR_BIT
    };

    m_image.emplace(m_device, m_context, imageProperties);

    VKCommandBuffer tempCommandBuffer{
        m_device, m_device->getGraphicsCommandPool()
    };
    VkQueue graphicsQueue = m_device->getQueues().graphics;

    tempCommandBuffer.createAndBeginSingleUse();

    m_image->transitionLayout(
      tempCommandBuffer, format, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    );

    m_image->copyFromBuffer(stagingBuffer, tempCommandBuffer);

    m_image->transitionLayout(
      tempCommandBuffer, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );

    tempCommandBuffer.endSingleUse(graphicsQueue);

    auto samplerInfo = createSamplerCreateInfo();
    VK_ASSERT(vkCreateSampler(
      m_device->getLogicalDevice(), &samplerInfo, m_context->getAllocator(),
      &m_sampler
    ));

    m_generation++;
    LOG_TRACE("Texture created: {}", m_props.name);
}

VKTexture::~VKTexture() {
    const auto logicalDevice = m_device->getLogicalDevice();

    vkDeviceWaitIdle(logicalDevice);
    vkDestroySampler(logicalDevice, m_sampler, m_context->getAllocator());

    LOG_TRACE("Texture destroyed: {}", m_props.name);
}

VKImage* VKTexture::getImage() { return m_image.get(); }

VkSampler VKTexture::getSampler() { return m_sampler; }

}  // namespace sl::vk
