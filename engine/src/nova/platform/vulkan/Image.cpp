#include "Image.h"

#include "Device.h"

namespace nova::platform::vulkan {

Image::Image(const Args& args) : m_size(args.size), m_device(args.device), m_context(args.context) {
    auto logicalDevice = m_device->getLogicalDevice();
    auto allocator     = m_context->getAllocator();

    createImage(args, logicalDevice, allocator);
    allocateAndBindMemory(args, m_device, allocator);

    if (args.createView) createView(args, logicalDevice, allocator);
}

Image::~Image() {
    auto logicalDevice = m_device->getLogicalDevice();
    auto allocator     = m_context->getAllocator();

    if (m_view) vkDestroyImageView(logicalDevice, m_view, allocator);
    if (m_memory) vkFreeMemory(logicalDevice, m_memory, allocator);
    if (m_handle) vkDestroyImage(logicalDevice, m_handle, allocator);
}

VkImageCreateInfo createImageCreateInfo(const Image::Args& args) {
    VkImageCreateInfo imageCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    imageCreateInfo.imageType         = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.width      = args.size.width;
    imageCreateInfo.extent.height     = args.size.height;
    imageCreateInfo.extent.depth      = 1;  // TODO: Support configurable depth.
    imageCreateInfo.mipLevels         = 4;  // TODO: Support mip mapping
    imageCreateInfo.arrayLayers       = 1;  // TODO: Support number of layers in the image.
    imageCreateInfo.format            = args.device->getDepthFormat();
    imageCreateInfo.tiling            = args.tiling;
    imageCreateInfo.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage             = args.usage;
    imageCreateInfo.samples           = VK_SAMPLE_COUNT_1_BIT;  // TODO: Configurable sample count.
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;    // TODO: Configurable sharing mode.

    return imageCreateInfo;
}

VkImageViewCreateInfo createViewCreateInfo(const Image::Args& args, VkImage imageHandle) {
    VkImageViewCreateInfo viewCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};

    viewCreateInfo.image                       = imageHandle;
    viewCreateInfo.viewType                    = VK_IMAGE_VIEW_TYPE_2D;
    viewCreateInfo.format                      = args.device->getDepthFormat();
    viewCreateInfo.subresourceRange.aspectMask = args.viewAspectFlags;

    viewCreateInfo.subresourceRange.baseMipLevel   = 0;
    viewCreateInfo.subresourceRange.levelCount     = 1;
    viewCreateInfo.subresourceRange.baseArrayLayer = 0;
    viewCreateInfo.subresourceRange.layerCount     = 1;

    return viewCreateInfo;
}

VkMemoryRequirements getMemoryRequirements(VkDevice device, VkImage handle) {
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(device, handle, &memoryRequirements);

    return memoryRequirements;
}

VkMemoryAllocateInfo createMemoryAllocateInfo(
    const VkMemoryRequirements& requirements, uint32_t memoryTypeIndex
) {
    VkMemoryAllocateInfo memoryAllocateInfo = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    memoryAllocateInfo.allocationSize       = requirements.size;
    memoryAllocateInfo.memoryTypeIndex      = memoryTypeIndex;

    return memoryAllocateInfo;
}

void Image::createImage(const Args& args, VkDevice logicalDevice, VkAllocator allocator) {
    auto imageCreateInfo = createImageCreateInfo(args);
    VK_ASSERT(vkCreateImage(logicalDevice, &imageCreateInfo, allocator, &m_handle));
}

void Image::allocateAndBindMemory(const Args& args, const Device* device, VkAllocator allocator) {
    auto logicalDevice = device->getLogicalDevice();

    auto memoryRequirements = getMemoryRequirements(logicalDevice, m_handle);
    auto memoryType = device->findMemoryIndex(memoryRequirements.memoryTypeBits, args.memoryFlags);

    if (not memoryType) LOG_ERROR("Required memory type not found. Image not valid.");

    auto memoryAllocateInfo = createMemoryAllocateInfo(memoryRequirements, memoryType.value_or(-1));
    VK_ASSERT(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, allocator, &m_memory));

    VK_ASSERT(vkBindImageMemory(logicalDevice, m_handle, m_memory, 0));
}

void Image::createView(const Args& args, VkDevice logicalDevice, VkAllocator allocator) {
    auto viewCreateInfo = createViewCreateInfo(args, m_handle);
    VK_ASSERT(vkCreateImageView(logicalDevice, &viewCreateInfo, allocator, &m_view));
}

}  // namespace nova::platform::vulkan
