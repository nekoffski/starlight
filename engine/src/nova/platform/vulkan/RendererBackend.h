#pragma once

#include "nova/gfx/RendererBacked.h"
#include "nova/core/fwd.h"
#include "nova/core/Window.h"

#include "Context.h"

namespace nova::platform::vulkan {

class RendererBackend : public gfx::RendererBackend {
   public:
    explicit RendererBackend(core::Window& window) : m_frameCount(0), m_context(window) {}

    ~RendererBackend() {}

    bool beginFrame() override { return true; }

    bool endFrame() override {
        ++m_frameCount;
        return true;
    }

   private:
    int m_frameCount;

    Context m_context;
};

}  // namespace nova::platform::vulkan