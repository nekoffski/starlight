#pragma once

#include <starlight/framework/graphics/LowLevelRenderer.h>
#include <starlight/framework/graphics/camera/Camera.h>

namespace starl::rendering::renderer {

using namespace framework;

class Renderer {
public:
    explicit Renderer(graphics::LowLevelRenderer& lowLevelRenderer)
        : m_lowLevelRenderer(lowLevelRenderer) {
    }

    virtual ~Renderer() = default;

protected:
    graphics::LowLevelRenderer& m_lowLevelRenderer;
};
}
