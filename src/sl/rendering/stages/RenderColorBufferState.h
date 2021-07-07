#pragma once

#include "sl/gfx/Shader.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/buffer/VertexArray.h"
#include "sl/rendering/Stage.h"
#include "sl/utils/Globals.h"

#include "sl/core/Logger.h"

namespace sl::rendering::stages {

class RenderColorBufferStage : public Stage {
public:
    explicit RenderColorBufferStage()
        : m_quadVao(GLOBALS().geom->frontSquareVAO.get())
        , m_colorBufferShader(gfx::Shader::load(
              GLOBALS().config.paths.shaders + "/ColorBuffer.vert", GLOBALS().config.paths.shaders + "/ColorBuffer.frag")) {
    }

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene, gfx::buffer::FrameBuffer*) override {
        auto [width, height] = m_windowProxy->getSize();
        gfx::ViewFrustum::Viewport viewport { width, height };

        renderer.setTemporaryViewport(viewport);

        m_colorBuffer->bind(0);
        m_colorBufferShader->enable();

        m_quadVao->bind();
        renderer.renderVertexArray(*m_quadVao);
        m_quadVao->unbind();

        m_colorBufferShader->disable();
        m_colorBuffer->unbind();
        renderer.restoreViewport();
    }

    void setColorBuffer(gfx::Texture* colorBuffer) {
        m_colorBuffer = colorBuffer;
    }

private:
    gfx::buffer::VertexArray* m_quadVao;
    gfx::Texture* m_colorBuffer;

    std::shared_ptr<gfx::Shader> m_colorBufferShader;
};

}