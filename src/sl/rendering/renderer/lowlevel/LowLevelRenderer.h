#pragma once

#include <unordered_map>

#include "sl/rendering/camera/Camera.h"

#include "sl/platform/gpu/GraphicsContext.h"
#include "sl/platform/gpu/RenderAPI.h"
#include "sl/platform/gpu/VertexArray.h"
#include "sl/platform/shader/Shader.h"
#include "sl/platform/texture/Texture.h"
#include "sl/platform/window/Window.h"

#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"

#include "sl/geometry/Mesh.h"

// TODO: create FWD and move implementations to .cpp

namespace sl::rendering::renderer::lowlevel {

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
    void renderVertexArray(std::shared_ptr<platform::gpu::VertexArray>);
    void renderVertexArrayWithDepthMaskDisabled(std::shared_ptr<platform::gpu::VertexArray>);

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
