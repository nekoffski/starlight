// #pragma once

// #include "Vulkan.h"

// #include "Device.h"
// #include "Context.h"

// namespace nova::platform::vulkan {

// struct CommandBuffer {
//     enum class State : unsigned char {
//         ready = 0,
//         recording,
//         inRenderPass,
//         recordingEnded,
//         submitted,
//         notAllocated
//     };

//     explicit CommandBuffer(const Device& device, const VkCommandPool& pool, bool isPrimary)
//         : device(device), pool(pool) {
//         create(device, pool, isPrimary);
//     }

//     ~CommandBuffer() { destroy(device, pool); }

//     void create(const Device& device, const VkCommandPool& pool, bool isPrimary) {
//         VkCommandBufferAllocateInfo allocate_info = {
//             VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
//         allocate_info.commandPool = pool;
//         allocate_info.level =
//             isPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
//         allocate_info.commandBufferCount = 1;
//         allocate_info.pNext              = 0;

//         state = State::notAllocated;
//         VK_CHECK(vkAllocateCommandBuffers(device.logicalDevice, &allocate_info, &handle));
//         state = State::ready;
//     }

//     void destroy(const Device& device, VkCommandPool pool) {
//         vkFreeCommandBuffers(device.logicalDevice, pool, 1, &handle);
//         state = State::notAllocated;
//     }

//     void begin(bool is_single_use, bool is_renderpass_continue, bool is_simultaneous_use) {
//         VkCommandBufferBeginInfo begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
//         begin_info.flags                    = 0;
//         if (is_single_use) {
//             begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
//         }
//         if (is_renderpass_continue) {
//             begin_info.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
//         }
//         if (is_simultaneous_use) {
//             begin_info.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
//         }

//         VK_CHECK(vkBeginCommandBuffer(handle, &begin_info));
//         state = State::recording;
//     }

//     void end() {
//         VK_CHECK(vkEndCommandBuffer(handle));
//         state = State::recordingEnded;
//     }

//     void update_submitted() { state = State::submitted; }

//     void reset() { state = State::ready; }

//     void createAndBeginSingleUse(const Device& device, const VkCommandPool& pool) {
//         create(device, pool, true);
//         begin(true, false, false);
//     }

//     void endSingleUse(const Device& device, const VkCommandPool& pool, VkQueue queue) {
//         // End the command buffer.
//         end();

//         // Submit the queue
//         VkSubmitInfo submit_info       = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
//         submit_info.commandBufferCount = 1;
//         submit_info.pCommandBuffers    = &handle;
//         VK_CHECK(vkQueueSubmit(queue, 1, &submit_info, 0));

//         // Wait for it to finish
//         VK_CHECK(vkQueueWaitIdle(queue));

//         // Free the command buffer.
//         destroy(device, pool);
//     }

//     const Device& device;
//     const VkCommandPool& pool;

//     VkCommandBuffer handle;
//     State state;
// };
// }  // namespace nova::platform::vulkan

// // namespace {

// // vk::raii::CommandBuffer allocateCommandBuffer(
// //     Device& device, vk::raii::CommandPool& pool, vk::CommandBufferLevel level
// // ) {
// //     vk::CommandBufferAllocateInfo info{};

// //     info.commandPool        = *pool;
// //     info.level              = level;
// //     info.commandBufferCount = 1;
// //     info.pNext              = nullptr;

// //     // TODO: need move here, consider storing all command buffer inside of the class
// //     return std::move(vk::raii::CommandBuffers{*device.getLogicalDevice(), info}.front());
// // }

// // }  // namespace

// //     explicit CommandBuffer(
// //         Device& device, vk::raii::CommandPool& pool, vk::CommandBufferLevel level
// //     )
// //         : handle(allocateCommandBuffer(device, pool, level)), state(State::notAllocated) {
// //         state = State::ready;
// //     }

// //     void beginSingleUse() { begin(true, false, false); }

// //     void endSingleUse(vk::raii::CommandPool& pool, vk::raii::Queue& queue) {
// //         end();

// //         vk::SubmitInfo info{};
// //         info.pCommandBuffers = &(*handle);

// //         queue.submit(info);
// //         queue.waitIdle();
// //     }

// //     void begin(bool singleUse, bool isRenderPassContinue, bool isSimultaneousUse) {
// //         vk::CommandBufferBeginInfo info{};

// //         if (singleUse) info.flags |= vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
// //         if (isRenderPassContinue) info.flags |=
// //         vk::CommandBufferUsageFlagBits::eRenderPassContinue; if (isSimultaneousUse) info.flags
// |=
// //         vk::CommandBufferUsageFlagBits::eSimultaneousUse;

// //         handle.begin(info);
// //         state = State::recording;
// //     }

// //     void end() {
// //         handle.end();
// //         state = State::recordingEnded;
// //     }

// //     void updateSubmitted() { state = State::submitted; }

// //     void reset() { state = State::ready; }

// //     State state;
// //     vk::raii::CommandBuffer handle;
// // };

// // }  // namespace nova::platform::vulkan
