#pragma once

#include "Vulkan.h"

#include "Device.h"

namespace nova::platform::vulkan {

namespace {

vk::raii::Fence createFence(const vk::raii::Device& device, bool createSignaled) {
    vk::FenceCreateInfo info{};

    if (createSignaled) info.flags = vk::FenceCreateFlagBits::eSignaled;

    return vk::raii::Fence{device, info};
}

}  // namespace

struct Fence {
    explicit Fence(const vk::raii::Device& device, bool createSignaled)
        : handle(createFence(device, createSignaled)), isSignaled(createSignaled) {}

    bool wait(const vk::raii::Device& device, uint64_t timeout) {
        if (isSignaled) return true;

        auto result = device.waitForFences(*handle, true, timeout);

        // TODO
        switch (result) {}

        return true;
    }

    void reset(const vk::raii::Device& device) {
        if (isSignaled) {
            device.resetFences(*handle);
            isSignaled = false;
        }
    }

    vk::raii::Fence handle;
    bool isSignaled;
};

}  // namespace nova::platform::vulkan