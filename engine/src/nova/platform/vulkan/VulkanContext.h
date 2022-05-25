#pragma once

#include <vulkan/vulkan.hpp>
#include <kc/core/Log.h>

namespace nova::platform::vulkan {

struct VulkanContext {
    explicit VulkanContext() {
        vk::ApplicationInfo applicationInfo{};
        applicationInfo.pApplicationName   = "Vulkan";
        applicationInfo.applicationVersion = 1;
        applicationInfo.pApplicationName   = "Nova";
        applicationInfo.engineVersion      = 1;
        applicationInfo.apiVersion         = VK_API_VERSION_1_2;

        vk::InstanceCreateInfo createInfo{};
        createInfo.pApplicationInfo        = &applicationInfo;
        createInfo.enabledExtensionCount   = 0;
        createInfo.ppEnabledExtensionNames = 0;
        createInfo.enabledLayerCount       = 0;
        createInfo.ppEnabledLayerNames     = 0;

        instance = vk::createInstance(createInfo, allocator);

        LOG_TRACE("Vulkan context initialized");
    }

    ~VulkanContext() { instance.destroy(); }

    vk::Instance instance;
    vk::AllocationCallbacks* allocator = nullptr;
};

}  // namespace nova::platform::vulkan
