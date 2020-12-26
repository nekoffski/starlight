#pragma once

#include <unordered_map>

#include "sl/graphics/camera/Camera.h"

#include "sl/graphics/GraphicsContext.h"
#include "sl/graphics/RenderApi.h"
#include "sl/graphics/buffer/VertexArray.h"
#include "sl/graphics/Shader.h"
#include "sl/graphics/Texture.h"

#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"

#include "sl/geometry/Mesh.h"

#include "Viewport.h"

namespace sl::graphics {

class Renderer {
public:
    explicit Renderer(std::shared_ptr<graphics::GraphicsContext> graphicsContext,
        std::unique_ptr<graphics::RenderApi> renderApi, Viewport viewport)
        : m_graphicsContext(graphicsContext)
        , m_renderApi(std::move(renderApi))
        , m_viewport(std::move(viewport)) {
    }

    void init();
    void renderVertexArray(std::shared_ptr<graphics::buffer::VertexArray>);
    // wtf? xd
    void renderVertexArrayWithDepthMaskDisabled(std::shared_ptr<graphics::buffer::VertexArray>);

    void clearBuffers(unsigned buff) {
        m_graphicsContext->clearBuffers(buff);
    }

    const math::Mat4& getProjectionMatrix() {
        return m_projectionMatrix;
    }

    void setTemporaryViewport(const Viewport& viewport) {
        m_graphicsContext->setViewport(viewport.width, viewport.height);
        calculateProjectionMatrix(viewport);
    }

    void restoreViewport() {
        m_graphicsContext->setViewport(m_viewport.width, m_viewport.height);
        calculateProjectionMatrix();
    }

    void setViewport(const Viewport& viewport) {
        m_viewport = viewport;
        restoreViewport();
        calculateProjectionMatrix();
    }

    void swapBuffers() {
        m_graphicsContext->swapBuffers();
    }

    void setProjectionMatrix(const math::Mat4& projectionMatrix) {
        m_projectionMatrix = projectionMatrix;
    }

private:
    std::shared_ptr<graphics::GraphicsContext> m_graphicsContext;
    std::unique_ptr<graphics::RenderApi> m_renderApi;
    math::Mat4 m_projectionMatrix;
    Viewport m_viewport;

    void calculateProjectionMatrix() {
        calculateProjectionMatrix(m_viewport);
    }

    void calculateProjectionMatrix(const Viewport& viewport) {
        float aspect = static_cast<float>(viewport.width) / m_viewport.height;
        m_projectionMatrix = math::perspective(viewport.fieldOfView, aspect, viewport.nearZ, viewport.farZ);
    }
};
}
