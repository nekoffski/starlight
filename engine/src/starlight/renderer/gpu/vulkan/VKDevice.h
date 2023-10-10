#pragma once

#include "Vulkan.h"

#include <kc/core/Log.h>

#include "VKContext.h"

namespace sl::vk {

class VKDevice {
public:
    struct SwapchainSupportInfo {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    // TODO: try to merge those structs somehow or give them more self-descriptive
    // names at least
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

    struct QueueIndices {
        int32_t graphics;
        int32_t present;
        int32_t transfer;
    };

    struct Queues {
        VkQueue graphics;
        VkQueue present;
        VkQueue transfer;
    };

    explicit VKDevice(VKContext* context);
    ~VKDevice();

    VkDevice getLogicalDevice() const;
    VkFormat getDepthFormat() const;

    const VkPhysicalDeviceProperties& getProperties() const;

    const SwapchainSupportInfo& getSwapchainSupport() const;
    const QueueIndices& getQueueIndices() const;
    const Queues& getQueues();

    // void querySwapchainSupport(VkSurfaceKHR surface);

    std::optional<int32_t> findMemoryIndex(
      uint32_t typeFilter, uint32_t propertyFlags
    ) const;

    void detectDepthFormat();

    VkCommandPool getGraphicsCommandPool() const;

    bool supportsDeviceLocalHostVisible() const;

private:
    void createCommandPool();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void assignQueues();

    void storeDevice(
      VkPhysicalDevice device, const DeviceProperties& properties,
      const DeviceInfo& info
    );

    std::vector<uint32_t> prepareQueuesIndices();

    VKContext* m_context;

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_logicalDevice          = VK_NULL_HANDLE;

    SwapchainSupportInfo m_swapchainSupport;
    QueueIndices m_queueIndices;
    Queues m_queues;

    VkCommandPool m_graphicsCommandPool = VK_NULL_HANDLE;

    VkPhysicalDeviceProperties m_properties;
    VkPhysicalDeviceFeatures m_features;
    VkPhysicalDeviceMemoryProperties m_memory;

    VkFormat m_depthFormat;

    bool m_supportsDeviceLocalHostVisible;
};

}  // namespace sl::vk
