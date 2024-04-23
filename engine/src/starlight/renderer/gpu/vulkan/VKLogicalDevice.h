#pragma once

#include "VKContext.h"
#include "VKPhysicalDevice.h"

namespace sl::vk {

class VKLogicalDevice {
public:
    struct Queues {
        VkQueue graphics;
        VkQueue present;
        VkQueue transfer;
    };

    explicit VKLogicalDevice(VKContext& context, VKPhysicalDevice& physicalDevice);
    ~VKLogicalDevice();

    const Queues& getQueues();
    VkFormat getDepthFormat() const;
    u8 getDepthChannelCount() const;
    VkCommandPool getGraphicsCommandPool();

    VkDevice getHandle();

private:
    void createLogicalDeviceInstance();
    void assignQueues();
    void createCommandPool();
    void detectDepthFormat();

    VKContext& m_context;
    VKPhysicalDevice& m_physicalDevice;

    VkDevice m_handle;
    VkCommandPool m_graphicsCommandPool;

    Queues m_queues;
    VkFormat m_depthFormat;
    u8 m_depthChannelCount;
};

}  // namespace sl::vk
