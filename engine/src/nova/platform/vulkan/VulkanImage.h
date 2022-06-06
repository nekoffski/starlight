#pragma once

#include <optional>

#include "Vulkan.h"

#include "VulkanDevice.h"

namespace nova::platform::vulkan {

static vk::raii::Image createImage(
    uint32_t w, uint32_t h, vk::ImageType type, vk::Format format, vk::ImageTiling tiling,
    vk::ImageUsageFlags usage, VulkanDevice& device
) {
    vk::ImageCreateInfo createInfo{};
    createInfo.imageType     = vk::ImageType::e2D;
    createInfo.extent.width  = w;
    createInfo.extent.height = h;
    createInfo.extent.depth  = 1;
    createInfo.mipLevels     = 4;
    createInfo.arrayLayers   = 1;
    createInfo.format        = format;
    createInfo.tiling        = tiling;
    createInfo.initialLayout = vk::ImageLayout::eUndefined;
    createInfo.usage         = usage;
    createInfo.samples       = vk::SampleCountFlagBits::e1;
    createInfo.sharingMode   = vk::SharingMode::eExclusive;

    return vk::raii::Image{*device.getLogicalDevice(), createInfo};
}

static vk::raii::DeviceMemory allocateDeviceMemory(
    VulkanDevice& device, vk::raii::Image& image, vk::MemoryPropertyFlags memoryFlags
) {
    auto requirements = image.getMemoryRequirements();

    vk::MemoryAllocateInfo info{};
    info.allocationSize  = requirements.size;
    info.memoryTypeIndex = device.findMemoryIndex(requirements.memoryTypeBits, memoryFlags);

    vk::raii::DeviceMemory memory{*device.getLogicalDevice(), info, nullptr};

    image.bindMemory(*memory, 0);

    return memory;
}

static vk::raii::ImageView createImageView(
    VulkanDevice& device, vk::Format format, vk::raii::Image& image,
    vk::ImageAspectFlags aspectFlags
) {
    vk::ImageViewCreateInfo createInfo{};

    createInfo.image                           = *image;
    createInfo.viewType                        = vk::ImageViewType::e2D;
    createInfo.format                          = format;
    createInfo.subresourceRange.aspectMask     = aspectFlags;
    createInfo.subresourceRange.baseMipLevel   = 0;
    createInfo.subresourceRange.levelCount     = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount     = 1;

    return vk::raii::ImageView{*device.getLogicalDevice(), createInfo, nullptr};
}

struct VulkanImage {
    explicit VulkanImage(
        uint32_t w, uint32_t h, vk::ImageType type, vk::Format format, vk::ImageTiling tiling,
        vk::ImageUsageFlags usage, vk::MemoryPropertyFlags memoryFlags,
        vk::ImageAspectFlags aspectFlags, VulkanDevice& device
    )
        : width(w)
        , height(h)
        , handle(createImage(w, h, type, format, tiling, usage, device))
        , memory(allocateDeviceMemory(device, handle, memoryFlags))
        , view(createImageView(device, format, handle, aspectFlags)) {
        LOG_TRACE("Created VulkanImage");
    }

    vk::raii::Image handle;
    vk::raii::DeviceMemory memory;
    vk::raii::ImageView view;

    uint32_t width;
    uint32_t height;
};

}  // namespace nova::platform::vulkan
