#pragma once

#include "nova/gfx/RendererBacked.h"

#include "VulkanContext.h"

namespace nova::platform::vulkan {

class VulkanRendererBackend : public gfx::RendererBackend {
   public:
    explicit VulkanRendererBackend() : m_frameCount(0) {}

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