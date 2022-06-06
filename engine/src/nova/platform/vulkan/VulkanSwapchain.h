#pragma once

#include <vector>
#include <optional>

#include <kc/core/Log.h>

#include "nova/math/Size.hpp"

#include "Vulkan.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"

namespace nova::platform::vulkan {

static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR>& formatsAvailable
) {
    for (const auto& availableFormat : formatsAvailable) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
            availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }

    return formatsAvailable[0];
}

static vk::PresentModeKHR chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR>& availablePresentModes
) {
    for (const auto& availablePresentMode : availablePresentModes)
        if (availablePresentMode == vk::PresentModeKHR::eImmediate) return availablePresentMode;

    return vk::PresentModeKHR::eFifo;
}

static vk::raii::SwapchainKHR createSwapChain(
    VulkanDevice& device, const SwapChainSupportDetails& swapChainSupport,
    vk::SurfaceFormatKHR imageFormat, vk::raii::SurfaceKHR& surface, const math::Size2i& screenSize
) {
    vk::Extent2D swapchainExtent;

    auto queuesIndices        = device.getQueueFamilies();
    auto& surfaceCapabilities = swapChainSupport.capabilities;

    if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max()) {
        swapchainExtent.width = std::clamp(
            static_cast<uint32_t>(screenSize.width), surfaceCapabilities.minImageExtent.width,
            surfaceCapabilities.maxImageExtent.width
        );
        swapchainExtent.height = std::clamp(
            static_cast<uint32_t>(screenSize.height), surfaceCapabilities.minImageExtent.height,
            surfaceCapabilities.maxImageExtent.height
        );
    } else {
        swapchainExtent = surfaceCapabilities.currentExtent;
    }

    auto swapChainPresentMode = chooseSwapPresentMode(swapChainSupport.presentModes);

    vk::SurfaceTransformFlagBitsKHR preTransform =
        (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
            ? vk::SurfaceTransformFlagBitsKHR::eIdentity
            : surfaceCapabilities.currentTransform;

    vk::CompositeAlphaFlagBitsKHR compositeAlpha =
        ((surfaceCapabilities.supportedCompositeAlpha &
          vk::CompositeAlphaFlagBitsKHR::ePreMultiplied)
             ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
         : (surfaceCapabilities.supportedCompositeAlpha &
            vk::CompositeAlphaFlagBitsKHR::ePostMultiplied)
             ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
         : (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit)
             ? vk::CompositeAlphaFlagBitsKHR::eInherit
             : vk::CompositeAlphaFlagBitsKHR::eOpaque);

    vk::SwapchainCreateInfoKHR swapChainCreateInfo(
        vk::SwapchainCreateFlagsKHR(), *surface, surfaceCapabilities.minImageCount,
        imageFormat.format, vk::ColorSpaceKHR::eSrgbNonlinear, swapchainExtent, 1,
        vk::ImageUsageFlagBits::eColorAttachment, vk::SharingMode::eExclusive, {}, preTransform,
        compositeAlpha, swapChainPresentMode, true, nullptr
    );

    std::array<uint32_t, 2> queueFamilyIndices = {
        queuesIndices.graphicsFamily.value(), queuesIndices.presentFamily.value()};

    if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
        swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        swapChainCreateInfo.queueFamilyIndexCount =
            static_cast<uint32_t>(queueFamilyIndices.size());
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    }

    return vk::raii::SwapchainKHR{*device.getLogicalDevice(), swapChainCreateInfo};
}

struct VulkanSwapchain {
    explicit VulkanSwapchain(
        VulkanDevice& device, vk::raii::SurfaceKHR& surface, const math::Size2i& screenSize
    )
        : m_swapChainSupportDetails(device.getSwapChainSupport(surface))
        , m_imageFormat(chooseSwapSurfaceFormat(m_swapChainSupportDetails.formats))
        , m_handle(
              createSwapChain(device, m_swapChainSupportDetails, m_imageFormat, surface, screenSize)
          )
        , m_depthAttachment(
              screenSize.width, screenSize.height, vk::ImageType::e2D, device.getDepthFormat(),
              vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment,
              vk::MemoryPropertyFlagBits::eDeviceLocal, vk::ImageAspectFlagBits::eDepth, device
          ) {
        createImages(device);

        LOG_TRACE("Vulkan Swapchain created");
    }

    std::optional<uint32_t> acquireNextImageIndex(
        VulkanDevice& device, uint64_t nsTimeout, vk::raii::Semaphore& imageAvailableSempaphore,
        vk::raii::Fence& fence
    ) {
        vk::AcquireNextImageInfoKHR acquireInfo(
            *m_handle, nsTimeout, *imageAvailableSempaphore, *fence
        );
        auto [code, result] = device.getLogicalDevice()->acquireNextImage2KHR(acquireInfo);

        if (code != vk::Result::eSuccess) {
            // recreate();
            return {};
        }

        return result;
    }

    void createImages(VulkanDevice& device) {
        auto images = m_handle.getImages();

        m_images.reserve(images.size());
        for (auto& image : images) m_images.push_back(static_cast<vk::Image>(image));

        m_views.reserve(m_images.size());
        vk::ImageViewCreateInfo imageViewCreateInfo(
            {}, {}, vk::ImageViewType::e2D, m_imageFormat.format, {},
            {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
        );

        vk::raii::Device& logicalDevice = *device.getLogicalDevice();

        for (auto& image : m_images) {
            imageViewCreateInfo.image = image;
            m_views.push_back({logicalDevice, imageViewCreateInfo});
        }
    }

    void present() {}

    SwapChainSupportDetails m_swapChainSupportDetails;

    vk::SurfaceFormatKHR m_imageFormat;
    uint8_t m_maxFramesInFlight = 2;
    vk::raii::SwapchainKHR m_handle;

    std::vector<vk::Image> m_images;
    std::vector<vk::raii::ImageView> m_views;

    VulkanImage m_depthAttachment;
};

}  // namespace nova::platform::vulkan
