// #pragma once

// #include "Vulkan.h"

// #include "Device.h"
// #include "Context.h"

// namespace nova::platform::vulkan {

// struct Fence {
//     explicit Fence(const Context& context, const Device& device, bool isSignaled)
//         : context(context), device(device), isSignaled(isSignaled) {
//         VkFenceCreateInfo fence_create_info = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};

//         if (isSignaled) fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

//         VK_CHECK(vkCreateFence(device.logicalDevice, &fence_create_info, context.allocator,
//         &handle)
//         );
//     }

//     ~Fence() {
//         if (handle) vkDestroyFence(device.logicalDevice, handle, context.allocator);
//     }

//     bool wait(uint64_t timeout) {
//         if (isSignaled) return true;

//         switch (vkWaitForFences(device.logicalDevice, 1, &handle, true, timeout)) {
//             case VK_SUCCESS:
//                 isSignaled = true;
//                 return true;
//             case VK_TIMEOUT:
//                 LOG_WARN("vk_fence_wait - Timed out");
//                 break;
//             case VK_ERROR_DEVICE_LOST:
//                 LOG_ERROR("vk_fence_wait - VK_ERROR_DEVICE_LOST.");
//                 break;
//             case VK_ERROR_OUT_OF_HOST_MEMORY:
//                 LOG_ERROR("vk_fence_wait - VK_ERROR_OUT_OF_HOST_MEMORY.");
//                 break;
//             case VK_ERROR_OUT_OF_DEVICE_MEMORY:
//                 LOG_ERROR("vk_fence_wait - VK_ERROR_OUT_OF_DEVICE_MEMORY.");
//                 break;
//             default:
//                 LOG_ERROR("vk_fence_wait - An unknown error has occurred.");
//                 break;
//         }

//         return false;
//     }

//     void reset() {
//         if (isSignaled) {
//             VK_CHECK(vkResetFences(device.logicalDevice, 1, &handle));
//             isSignaled = false;
//         }
//     }

//     VkFence handle;
//     bool isSignaled;

//     const Context& context;
//     const Device& device;
// };

// }  // namespace nova::platform::vulkan
