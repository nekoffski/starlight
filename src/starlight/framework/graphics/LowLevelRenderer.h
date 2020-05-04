#pragma once

#include <unordered_map>

#include <starlight/framework/graphics/camera/Camera.h>

#include <starlight/platform/gpu/GraphicsContext.h>
#include <starlight/platform/gpu/RenderAPI.h>
#include <starlight/platform/gpu/VertexArray.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/platform/texture/Texture.h>
#include <starlight/platform/window/Window.h>

#include <starlight/math/Matrix.hpp>
#include <starlight/math/Utils.hpp>

#include <starlight/geometry/Mesh.h>

// TODO: create FWD and move implementations to .cpp

namespace starl::framework::graphics {

class LowLevelRenderer {
public:
    explicit LowLevelRenderer(std::unique_ptr<platform::window::Window>& window)
        : m_graphicsContext(platform::gpu::GraphicsContext::create(window->getHandle()))
        , m_renderAPI(platform::gpu::RenderAPI::create())
        , m_mainCamera(nullptr)
        , m_viewport(window->getParams().viewport) {
        calculateProjectionMatrix();
    }

    void init();
    void renderVertexArray(std::shared_ptr<platform::gpu::VertexArray>&);
    void renderVertexArrayWithDepthMaskDisabled(std::shared_ptr<platform::gpu::VertexArray>&);

    void begin();
    void end();

    const math::Mat4& getProjectionMatrix() {
        return m_projectionMatrix;
    }

private:
    std::unique_ptr<platform::gpu::GraphicsContext> m_graphicsContext;
    std::unique_ptr<platform::gpu::RenderAPI> m_renderAPI;
    std::shared_ptr<camera::Camera> m_mainCamera;
    platform::window::Viewport m_viewport;
    math::Mat4 m_projectionMatrix;

    void calculateProjectionMatrix() {
        float aspect = static_cast<float>(m_viewport.width) / m_viewport.height;
        m_projectionMatrix = math::perspective(m_viewport.fov, aspect, m_viewport.nearZ, m_viewport.farZ);
    }
};
}
