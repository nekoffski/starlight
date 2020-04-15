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

    void setCamera(std::shared_ptr<graphics::camera::Camera> camera) {
        m_camera = camera;
    }

protected:
    graphics::LowLevelRenderer& m_lowLevelRenderer;
    std::shared_ptr<graphics::camera::Camera> m_camera;
};
}
