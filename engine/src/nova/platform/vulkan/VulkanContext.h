#pragma once

#include <vector>

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan.h>

#include <kc/core/Log.h>
#include <kc/core/Utils.hpp>

#include "Utils.h"

#ifndef DEBUG
#define DEBUG
#endif

namespace nova::platform::vulkan {

struct VulkanContext {
    explicit VulkanContext();

    ~VulkanContext();

    vk::Instance instance;
    vk::AllocationCallbacks* allocator = nullptr;

#ifdef DEBUG
    vk::DebugUtilsMessengerEXT debugMessenger;

#endif

    vk::DynamicLoader dl;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
};

}  // namespace nova::platform::vulkan
