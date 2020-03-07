#pragma once

#include <starlight/platform/gpu/GraphicsContext.h>
#include <starlight/platform/window/Window.h>

namespace starl::framework::graphics {

class LowLevelRenderer {
public:
    explicit LowLevelRenderer(std::unique_ptr<platform::window::Window>& window)
        : m_graphicsContext(platform::gpu::GraphicsContext::create(window->getHandle())) {
    }

private:
    std::unique_ptr<platform::gpu::GraphicsContext> m_graphicsContext;
};
}
