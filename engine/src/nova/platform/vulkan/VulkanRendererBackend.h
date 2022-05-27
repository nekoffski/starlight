#pragma once

#include "nova/gfx/RendererBacked.h"
#include "nova/core/fwd.h"
#include "nova/core/Window.h"

#include "VulkanContext.h"

namespace nova::platform::vulkan {

class VulkanRendererBackend : public gfx::RendererBackend {
   public:
    explicit VulkanRendererBackend(core::Window& window) : m_frameCount(0), m_context(window) {}

    ~VulkanRendererBackend() {}

    bool beginFrame() override { return true; }

    bool endFrame() override {
        ++m_frameCount;
        return true;
    }

   private:
    int m_frameCount;

    VulkanContext m_context;
};

}  // namespace nova::platform::vulkan