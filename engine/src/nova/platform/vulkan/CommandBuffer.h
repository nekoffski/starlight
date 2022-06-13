#pragma once

#include "Vulkan.h"

#include "Device.h"

namespace nova::platform::vulkan {

namespace {

vk::raii::CommandBuffer allocateCommandBuffer(
    Device& device, vk::raii::CommandPool& pool, vk::CommandBufferLevel level
) {
    vk::CommandBufferAllocateInfo info{};

    info.commandPool        = *pool;
    info.level              = level;
    info.commandBufferCount = 1;
    info.pNext              = nullptr;

    // TODO: need move here, consider storing all command buffer inside of the class
    return std::move(vk::raii::CommandBuffers{*device.getLogicalDevice(), info}.front());
}

}  // namespace

struct CommandBuffer {
    enum class State : unsigned char {
        ready = 0,
        recording,
        inRenderPass,
        recordingEnded,
        submitted,
        notAllocated
    };

    explicit CommandBuffer(
        Device& device, vk::raii::CommandPool& pool, vk::CommandBufferLevel level
    )
        : handle(allocateCommandBuffer(device, pool, level)), state(State::notAllocated) {
        state = State::ready;
    }

    void beginSingleUse() { begin(true, false, false); }

    void endSingleUse(vk::raii::CommandPool& pool, vk::raii::Queue& queue) {
        end();

        vk::SubmitInfo info{};
        info.pCommandBuffers = &(*handle);

        queue.submit(info);
        queue.waitIdle();
    }

    void begin(bool singleUse, bool isRenderPassContinue, bool isSimultaneousUse) {
        vk::CommandBufferBeginInfo info{};

        if (singleUse) info.flags |= vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        if (isRenderPassContinue) info.flags |= vk::CommandBufferUsageFlagBits::eRenderPassContinue;
        if (isSimultaneousUse) info.flags |= vk::CommandBufferUsageFlagBits::eSimultaneousUse;

        handle.begin(info);
        state = State::recording;
    }

    void end() {
        handle.end();
        state = State::recordingEnded;
    }

    void updateSubmitted() { state = State::submitted; }

    void reset() { state = State::ready; }

    State state;
    vk::raii::CommandBuffer handle;
};

}  // namespace nova::platform::vulkan
