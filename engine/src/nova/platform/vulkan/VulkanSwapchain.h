#pragma once

#include <vector>
#include <optional>

#include "Vulkan.h"

#include "VulkanDevice.h"

namespace nova::platform::vulkan {

struct VulkanSwapchain {
    void recreate() {}

    std::optional<uint32_t> acquireNextImageIndex(
        VulkanDevice& device, uint64_t nsTimeout, vk::raii::Semaphore& imageAvailableSempaphore,
        vk::raii::Fence& fence
    ) {
        vk::AcquireNextImageInfoKHR acquireInfo(
            *m_handle, nsTimeout, *imageAvailableSempaphore, *fence
        );
        auto [code, result] = device.getLogicalDevice()->acquireNextImage2KHR(acquireInfo);

        if (code != vk::Result::eSuccess) {
            recreate();
            return {};
        }

        return result;
    }

    void present() {}

    vk::SurfaceFormatKHR m_imageFormat;
    uint8_t m_maxFramesInFlight;
    vk::raii::SwapchainKHR m_handle;

    std::vector<vk::Image> m_images;
    std::vector<vk::ImageView> m_views;
};

}  // namespace nova::platform::vulkan
