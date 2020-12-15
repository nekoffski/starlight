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

namespace sl::rendering {

class Renderer {
public:
    explicit Renderer(std::shared_ptr<platform::gpu::GraphicsContext> graphicsContext, std::unique_ptr<platform::gpu::RenderAPI> renderApi)
        : m_graphicsContext(graphicsContext)
        , m_renderApi(std::move(renderApi)) {
    }

    void init();
    void renderVertexArray(std::shared_ptr<platform::gpu::VertexArray>);
    // wtf? xd
    void renderVertexArrayWithDepthMaskDisabled(std::shared_ptr<platform::gpu::VertexArray>);

    void clearBuffers(unsigned buff) {
        m_graphicsContext->clearBuffers(buff);
    }

    const math::Mat4& getProjectionMatrix() {
        return m_projectionMatrix;
    }

    void setViewport(const platform::window::Viewport& viewport) {
        m_viewport = viewport;
    }

    void setTemporaryViewport(unsigned w, unsigned h) {
        m_graphicsContext->setViewport(w, h);
    }

    void restoreViewport() {
        m_graphicsContext->setViewport(m_viewport.width, m_viewport.height);
    }

    void swapBuffers() {
        m_graphicsContext->swapBuffers();
    }

    void setProjectionMatrix(const math::Mat4& projectionMatrix) {
        m_projectionMatrix = projectionMatrix;
    }

private:
    std::shared_ptr<platform::gpu::GraphicsContext> m_graphicsContext;
    std::unique_ptr<platform::gpu::RenderAPI> m_renderApi;
    platform::window::Viewport m_viewport;
    math::Mat4 m_projectionMatrix;

    void calculateProjectionMatrix() {
        float aspect = static_cast<float>(m_viewport.width) / m_viewport.height;
        m_projectionMatrix = math::perspective(m_viewport.fov, aspect, m_viewport.nearZ, m_viewport.farZ);
    }
};
}
