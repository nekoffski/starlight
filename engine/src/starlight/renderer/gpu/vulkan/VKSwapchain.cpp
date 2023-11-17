#include "VKSwapchain.h"

#include <kc/core/Log.h>
#include <kc/core/Utils.hpp>

#include "VKDevice.h"
#include "VKImage.h"
#include "VKFramebuffer.h"

namespace sl::vk {

VKSwapchain::VKSwapchain(
  VKDevice* device, VKContext* context, const Size2u32& size
) :
    m_device(device),
    m_context(context), m_viewportSize(size) {
    create();
}

VkSurfaceFormatKHR pickSurfaceFormat(
  const VKDevice::SwapchainSupportInfo& swapchainSupport
) {
    const auto demandedFormat     = VK_FORMAT_B8G8R8A8_UNORM;
    const auto demandedColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

    for (const auto& format : swapchainSupport.formats)
        if (format.format == demandedFormat && format.colorSpace == demandedColorSpace)
            return format;

    return swapchainSupport.formats[0];
}

VkPresentModeKHR pickPresentMode(
  const VKDevice::SwapchainSupportInfo& swapchainSupport
) {
    const auto defaultPresentMode  = VK_PRESENT_MODE_FIFO_KHR;
    const auto demandedPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;

    if (kc::core::contains(swapchainSupport.presentModes, demandedPresentMode))
        return demandedPresentMode;

    return defaultPresentMode;
}

VkExtent2D createSwapchainExtent(
  const Size2u32& viewportSize,
  const VKDevice::SwapchainSupportInfo& swapchainSupport
) {
    VkExtent2D swapchainExtent = { viewportSize.width, viewportSize.height };

    if (swapchainSupport.capabilities.currentExtent.width != UINT32_MAX)
        swapchainExtent = swapchainSupport.capabilities.currentExtent;

    // Clamp to the value allowed by the GPU.bonestent.width, min.width, max.width);
    auto& min = swapchainSupport.capabilities.minImageExtent;
    auto& max = swapchainSupport.capabilities.minImageExtent;

    swapchainExtent.height =
      std::clamp(swapchainExtent.height, min.height, max.height);

    return swapchainExtent;
}

std::vector<VKFramebuffer>* VKSwapchain::getFramebuffers() {
    return &m_framebuffers;
}

VKImage* VKSwapchain::getDepthBuffer() { return m_depthBuffer.get(); }

uint32_t getDeviceImageCount(const VKDevice::SwapchainSupportInfo& swapchainSupport
) {
    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;

    if (swapchainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapchainSupport.capabilities.maxImageCount) {
        imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    return imageCount;
}

VkSurfaceFormatKHR VKSwapchain::getSurfaceFormat() const { return m_imageFormat; }

std::span<LocalPtr<VKTexture>> VKSwapchain::getTextures() { return m_textures; }

struct SwapchainCreateInfo {
    VkSwapchainCreateInfoKHR handle;
    std::vector<uint32_t> queueFamilyIndices;
};

SwapchainCreateInfo createSwapchainCreateInfo(
  VKDevice* device, VkSurfaceKHR surface, uint32_t imageCount,
  VkSurfaceFormatKHR imageFormat, VkExtent2D extent, VkPresentModeKHR presentMode
) {
    SwapchainCreateInfo createInfo;
    auto& handle = createInfo.handle;

    handle                  = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
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

    handle.preTransform =
      device->getSwapchainSupport().capabilities.currentTransform;
    handle.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    handle.presentMode    = presentMode;
    handle.clipped        = VK_TRUE;
    handle.oldSwapchain   = 0;

    return createInfo;
}

VkImageViewCreateInfo createImageViewCreateInfo(VkImage image, VkFormat format) {
    VkImageViewCreateInfo viewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
    viewInfo.image                 = image;
    viewInfo.viewType              = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format                = format;
    viewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel   = 0;
    viewInfo.subresourceRange.levelCount     = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount     = 1;

    return viewInfo;
}

void VKSwapchain::createSwapchain() {
    const auto& swapchainSupport = m_device->getSwapchainSupport();

    m_imageFormat    = pickSurfaceFormat(swapchainSupport);
    auto presentMode = pickPresentMode(swapchainSupport);

    // TODO: what is the point?
    // device.swapchain_support = queryDeviceSwapchainSupport(device.physicalDevice,
    // context.surface);

    m_swapchainExtent     = createSwapchainExtent(m_viewportSize, swapchainSupport);
    auto deviceImageCount = getDeviceImageCount(swapchainSupport);

    auto swapchainCreateInfo = createSwapchainCreateInfo(
      m_device, m_context->getSurface(), deviceImageCount, m_imageFormat,
      m_swapchainExtent, presentMode
    );

    VK_ASSERT(vkCreateSwapchainKHR(
      m_device->getLogicalDevice(), &swapchainCreateInfo.handle,
      m_context->getAllocator(), &m_handle
    ));
}

void VKSwapchain::createImages() {
    const auto logicalDevice = m_device->getLogicalDevice();
    const auto allocator     = m_context->getAllocator();

    VK_ASSERT(vkGetSwapchainImagesKHR(logicalDevice, m_handle, &m_imageCount, 0));

    ASSERT(m_imageCount > 0, "swapchainImageCount==0 for vulkan swapchain");

    std::array<VkImage, 16> swapchainImages = { 0 };

    VK_ASSERT(vkGetSwapchainImagesKHR(
      logicalDevice, m_handle, &m_imageCount, swapchainImages.data()
    ));

    if (m_textures.size() != m_imageCount) {
        m_textures.clear();
        m_textures.resize(m_imageCount);

        VKTexture::Properties props{};
        props.width         = m_swapchainExtent.width;
        props.height        = m_swapchainExtent.height;
        props.channels      = 4;
        props.isTransparent = false;
        props.isWritable    = true;

        for (int i = 0; i < m_imageCount; ++i) {
            auto& swapchainImageHandle = swapchainImages[i];
            props.name = fmt::format("SL_InternalSwapchainTexture_{}", i);

            m_textures[i].emplace(
              m_context, m_device, props, static_cast<u32>(i + 2048),
              swapchainImageHandle, m_imageFormat.format
            );
        }
    } else {
        for (int i = 0; i < m_imageCount; ++i) {
            auto& swapchainImageHandle = swapchainImages[i];
            auto& texture              = m_textures[i];

            texture->resize(m_swapchainExtent.width, m_swapchainExtent.height);
        }
    }

    // Create depth image and its view.
    VKImage::Properties imageProperties{
        VK_IMAGE_TYPE_2D,
        m_swapchainExtent.width,
        m_swapchainExtent.height,
        m_device->getDepthFormat(),
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        true,
        VK_IMAGE_ASPECT_DEPTH_BIT,
    };

    m_depthBuffer.emplace(m_device, m_context, imageProperties);
}

void VKSwapchain::create() {
    createSwapchain();
    createImages();
    LOG_INFO("VKSwapchain created successfully.");
}

VKSwapchain::~VKSwapchain() { destroy(); }

void VKSwapchain::changeSize(const Size2u32& size) {
    m_viewportSize = size;
    // TODO: Consider recreating swapchain here as a trigger
    //       Investigate if it is required to wait for some VKFence e.g.
}

void VKSwapchain::destroy() {
    vkDestroySwapchainKHR(
      m_device->getLogicalDevice(), m_handle, m_context->getAllocator()
    );
    LOG_TRACE("VKSwapchain destroyed");
}

std::optional<uint32_t> VKSwapchain::acquireNextImageIndex(
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
  VkSemaphore* renderSemaphore, VkSwapchainKHR* swapchain,
  uint32_t* presentImageIndex
) {
    // Return the image to the swapchain for presentation.
    VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = renderSemaphore;
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains        = swapchain;
    presentInfo.pImageIndices      = presentImageIndex;
    presentInfo.pResults           = 0;

    return presentInfo;
}

void VKSwapchain::present(
  VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderSemaphore,
  uint32_t presentImageIndex
) {
    const auto presentInfo =
      createPresentInfo(&renderSemaphore, &m_handle, &presentImageIndex);

    if (VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);
        result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        // VKSwapchain is out of date, suboptimal or a framebuffer resize has
        // occurred. Trigger swapchain recreation
        recreate();
    } else if (result != VK_SUCCESS) {
        LOG_FATAL("Failed to present swap chain image!");
    }
}

void VKSwapchain::recreate() {
    destroy();
    create();
}

u32 VKSwapchain::getImageCount() const { return m_imageCount; }

}  // namespace sl::vk
