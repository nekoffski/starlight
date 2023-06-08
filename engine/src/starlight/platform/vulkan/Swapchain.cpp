#include "Swapchain.h"

#include <kc/core/Log.h>
#include <kc/core/Utils.hpp>

#include "Device.h"
#include "Image.h"
#include "Framebuffer.h"

namespace sl::platform::vulkan {

Swapchain::Swapchain(Device* device, Context* context, const math::Size2u32& size)
    : m_device(device), m_context(context), m_viewportSize(size) {
    create();
}

VkSurfaceFormatKHR pickSurfaceFormat(const Device::SwapchainSupportInfo& swapchainSupport) {
    const auto demandedFormat     = VK_FORMAT_B8G8R8A8_UNORM;
    const auto demandedColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

    for (const auto& format : swapchainSupport.formats)
        if (format.format == demandedFormat && format.colorSpace == demandedColorSpace)
            return format;

    return swapchainSupport.formats[0];
}

VkPresentModeKHR pickPresentMode(const Device::SwapchainSupportInfo& swapchainSupport) {
    const auto defaultPresentMode  = VK_PRESENT_MODE_FIFO_KHR;
    const auto demandedPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;

    if (kc::core::contains(swapchainSupport.presentModes, demandedPresentMode))
        return demandedPresentMode;

    return defaultPresentMode;
}

VkExtent2D createSwapchainExtent(
    const math::Size2u32& viewportSize, const Device::SwapchainSupportInfo& swapchainSupport
) {
    VkExtent2D swapchainExtent = {viewportSize.width, viewportSize.height};

    if (swapchainSupport.capabilities.currentExtent.width != UINT32_MAX)
        swapchainExtent = swapchainSupport.capabilities.currentExtent;

    // Clamp to the value allowed by the GPU.bonestent.width, min.width, max.width);
    auto& min = swapchainSupport.capabilities.minImageExtent;
    auto& max = swapchainSupport.capabilities.minImageExtent;

    swapchainExtent.height = std::clamp(swapchainExtent.height, min.height, max.height);

    return swapchainExtent;
}

uint64_t Swapchain::getImagesSize() const { return m_images.size(); }

std::vector<Framebuffer>* Swapchain::getFramebuffers() { return &m_framebuffers; }

Image* Swapchain::getDepthBuffer() { return m_depthBuffer.get(); }

std::vector<VkImageView>* Swapchain::getImageViews() { return &m_views; }

uint32_t getDeviceImageCount(const Device::SwapchainSupportInfo& swapchainSupport) {
    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;

    if (swapchainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapchainSupport.capabilities.maxImageCount) {
        imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    return imageCount;
}

VkSurfaceFormatKHR Swapchain::getSurfaceFormat() const { return m_imageFormat; }

struct SwapchainCreateInfo {
    VkSwapchainCreateInfoKHR handle;
    std::vector<uint32_t> queueFamilyIndices;
};

SwapchainCreateInfo createSwapchainCreateInfo(
    Device* device, VkSurfaceKHR surface, uint32_t imageCount, VkSurfaceFormatKHR imageFormat,
    VkExtent2D extent, VkPresentModeKHR presentMode
) {
    SwapchainCreateInfo createInfo;
    auto& handle = createInfo.handle;

    handle                  = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    handle.surface          = surface;
    handle.minImageCount    = imageCount;
    handle.imageFormat      = imageFormat.format;
    handle.imageColorSpace  = imageFormat.colorSpace;
    handle.imageExtent      = extent;
    handle.imageArrayLayers = 1;
    handle.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    const auto& queueIndices = device->getQueueIndices();

    createInfo.queueFamilyIndices = {
        static_cast<uint32_t>(queueIndices.graphics),
        static_cast<uint32_t>(queueIndices.present),
    };

    // Setup the queue family indices
    if (queueIndices.graphics != queueIndices.present) {
        handle.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        handle.queueFamilyIndexCount = 2;
        handle.pQueueFamilyIndices   = createInfo.queueFamilyIndices.data();
    } else {
        handle.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        handle.queueFamilyIndexCount = 0;
        handle.pQueueFamilyIndices   = 0;
    }

    handle.preTransform   = device->getSwapchainSupport().capabilities.currentTransform;
    handle.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    handle.presentMode    = presentMode;
    handle.clipped        = VK_TRUE;
    handle.oldSwapchain   = 0;

    return createInfo;
}

VkImageViewCreateInfo createImageViewCreateInfo(VkImage image, VkFormat format) {
    VkImageViewCreateInfo viewInfo           = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    viewInfo.image                           = image;
    viewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format                          = format;
    viewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel   = 0;
    viewInfo.subresourceRange.levelCount     = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount     = 1;

    return viewInfo;
}

void Swapchain::createSwapchain() {
    const auto& swapchainSupport = m_device->getSwapchainSupport();

    m_imageFormat    = pickSurfaceFormat(swapchainSupport);
    auto presentMode = pickPresentMode(swapchainSupport);

    // TODO: what is the point?
    // device.swapchain_support = queryDeviceSwapchainSupport(device.physicalDevice,
    // context.surface);

    m_swapchainExtent     = createSwapchainExtent(m_viewportSize, swapchainSupport);
    auto deviceImageCount = getDeviceImageCount(swapchainSupport);

    auto swapchainCreateInfo = createSwapchainCreateInfo(
        m_device, m_context->getSurface(), deviceImageCount, m_imageFormat, m_swapchainExtent,
        presentMode
    );

    VK_ASSERT(vkCreateSwapchainKHR(
        m_device->getLogicalDevice(), &swapchainCreateInfo.handle, m_context->getAllocator(),
        &m_handle
    ));
}

void Swapchain::createImages() {
    const auto logicalDevice = m_device->getLogicalDevice();
    const auto allocator     = m_context->getAllocator();

    uint32_t swapchainImageCount;
    VK_ASSERT(vkGetSwapchainImagesKHR(logicalDevice, m_handle, &swapchainImageCount, 0));

    ASSERT(swapchainImageCount > 0, "swapchainImageCount==0 for vulkan swapchain");

    m_images.resize(swapchainImageCount);
    m_views.resize(swapchainImageCount);

    VK_ASSERT(
        vkGetSwapchainImagesKHR(logicalDevice, m_handle, &swapchainImageCount, m_images.data())
    );

    // Views
    for (uint32_t i = 0; i < swapchainImageCount; ++i) {
        auto viewCreateInfo = createImageViewCreateInfo(m_images[i], m_imageFormat.format);
        VK_ASSERT(vkCreateImageView(logicalDevice, &viewCreateInfo, allocator, &m_views[i]));
    }

    // Create depth image and its view.
    Image::Properties imageProperties{
        VK_IMAGE_TYPE_2D,
        math::Size2u32(m_swapchainExtent.width, m_swapchainExtent.height),
        m_device->getDepthFormat(),
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        true,
        VK_IMAGE_ASPECT_DEPTH_BIT,
    };

    m_depthBuffer = std::make_unique<Image>(m_device, m_context, imageProperties);
}

void Swapchain::create() {
    createSwapchain();
    createImages();
    LOG_INFO("Swapchain created successfully.");
}

Swapchain::~Swapchain() { destroy(); }

void Swapchain::changeSize(const math::Size2u32& size) {
    m_viewportSize = size;
    // TODO: Consider recreating swapchain here as a trigger
    //       Investigate if it is required to wait for some Fence e.g.
}

void Swapchain::destroy() {
    const auto logicalDevice = m_device->getLogicalDevice();
    const auto allocator     = m_context->getAllocator();

    for (auto& imageView : m_views) vkDestroyImageView(logicalDevice, imageView, allocator);

    vkDestroySwapchainKHR(logicalDevice, m_handle, allocator);
}

std::optional<uint32_t> Swapchain::acquireNextImageIndex(
    Nanoseconds timeout, VkSemaphore imageSemaphore, VkFence fence
) {
    uint32_t index;

    VkResult result = vkAcquireNextImageKHR(
        m_device->getLogicalDevice(), m_handle, timeout, imageSemaphore, fence, &index
    );

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        // Trigger swapchain recreation, then boot out of the render loop.
        recreate();
        return {};
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        LOG_FATAL("Failed to acquire swapchain image!");
        return {};
    }

    return index;
}

VkPresentInfoKHR createPresentInfo(
    VkSemaphore* renderSemaphore, VkSwapchainKHR* swapchain, uint32_t* presentImageIndex
) {
    // Return the image to the swapchain for presentation.
    VkPresentInfoKHR presentInfo = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = renderSemaphore;
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains        = swapchain;
    presentInfo.pImageIndices      = presentImageIndex;
    presentInfo.pResults           = 0;

    return presentInfo;
}

void Swapchain::present(
    VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderSemaphore,
    uint32_t presentImageIndex
) {
    const auto presentInfo = createPresentInfo(&renderSemaphore, &m_handle, &presentImageIndex);

    if (VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);
        result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        // Swapchain is out of date, suboptimal or a framebuffer resize has occurred. Trigger
        // swapchain recreation
        recreate();
    } else if (result != VK_SUCCESS) {
        LOG_FATAL("Failed to present swap chain image!");
    }
}

void Swapchain::recreate() {
    destroy();
    create();
}

}  // namespace sl::platform::vulkan
