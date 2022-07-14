#pragma once

#include "Vulkan.h"

#include <kc/core/Log.h>

#include "Context.h"

namespace nova::platform::vulkan {

class Device {
   public:
    struct SwapchainSupportInfo {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    // TODO: try to merge those structs somehow or give them more self-descriptive names at least
    struct DeviceProperties {
        VkPhysicalDeviceProperties device;
        VkPhysicalDeviceFeatures features;
        VkPhysicalDeviceMemoryProperties memory;
    };

    struct PhysicalDeviceRequirements {
        bool graphics;
        bool present;
        bool compute;
        bool transfer;
        std::vector<const char*> deviceExtensionsNames;
        bool samplerAnisotropy;
        bool discreteGpu;
    };

    struct PhysicalDeviceQueueFamilyInfo {
        uint32_t graphics = -1;
        uint32_t present  = -1;
        uint32_t compute  = -1;
        uint32_t transfer = -1;
    };

    struct DeviceInfo {
        PhysicalDeviceQueueFamilyInfo queue;
        SwapchainSupportInfo swapchainSupport;
    };

    explicit Device(Context* context);
    ~Device();

    // void querySwapchainSupport(VkSurfaceKHR surface);

    std::optional<int32_t> findMemoryIndex(uint32_t typeFilter, uint32_t propertyFlags) const {
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memoryProperties);

        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
            bool isSuitable =
                (typeFilter & (1 << i)) &&
                (memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags;

            if (isSuitable) return i;
        }

        LOG_WARN("Unable to find suitable memory type!");
        return {};
    }

    bool detectDepthFormat() {
        std::vector<VkFormat> candidates{
            VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};
        uint32_t flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

        for (const auto& format : candidates) {
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &properties);

            if (((properties.linearTilingFeatures & flags) == flags) ||
                ((properties.optimalTilingFeatures & flags) == flags)) {
                m_depthFormat = format;
                return true;
            }
        }

        return false;
    }

   private:
    void createCommandPool();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void getQueues();

    void storeDevice(
        VkPhysicalDevice device, const DeviceProperties& properties, const DeviceInfo& info
    );

    std::vector<uint32_t> prepareQueuesIndices();

    Context* m_context;

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_logicalDevice          = VK_NULL_HANDLE;

    SwapchainSupportInfo m_swapchainSupport;
    int32_t m_graphicsQueueIndex;
    int32_t m_presentQueueIndex;
    int32_t m_transferQueueIndex;

    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue  = VK_NULL_HANDLE;
    VkQueue m_transferQueue = VK_NULL_HANDLE;

    VkCommandPool m_graphicsCommandPool = VK_NULL_HANDLE;

    VkPhysicalDeviceProperties m_properties;
    VkPhysicalDeviceFeatures m_features;
    VkPhysicalDeviceMemoryProperties m_memory;

    VkFormat m_depthFormat;
};

}  // namespace nova::platform::vulkan
