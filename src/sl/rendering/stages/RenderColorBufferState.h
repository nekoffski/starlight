#pragma once

#include "sl/gfx/Shader.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/buffer/VertexArray.h"
#include "sl/rendering/Stage.h"
#include "sl/utils/Globals.h"

#include "sl/async/TaskManager.h"
#include "sl/core/Logger.h"

namespace sl::rendering::stages {

class RenderColorBufferStage : public Stage {
public:
    explicit RenderColorBufferStage()
        : m_quadVao(GLOBALS().geom->frontSquareVAO.get())
        , m_colorBufferShader(gfx::Shader::load(
              GLOBALS().config.paths.shaders + "/ColorBuffer.vert", GLOBALS().config.paths.shaders + "/ColorBuffer.frag")) {

        // clang-format off
        #ifdef DEV_MODE
            ASYNC_ENGINE().addPeriodicTask<gfx::RecompileShaderOnUpdate>(m_colorBufferShader);
        #endif
        // clang-format on
    }

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene, gfx::buffer::FrameBuffer*) override {
        auto [width, height] = m_windowProxy->getSize();
        gfx::ViewFrustum::Viewport viewport { width, height };

        renderer.setTemporaryViewport(viewport);
        m_colorBufferShader->enable();

        m_colorBuffer->bind(1);
        m_bloomBuffer->bind(2);

        auto& visualSettings = GLOBALS().visual;

        m_colorBufferShader->setUniform("gamma", visualSettings.gammaCorrection);
        m_colorBufferShader->setUniform("exposure", visualSettings.exposure);
        m_colorBufferShader->setUniform("enableBloom", visualSettings.enableBloom);

        m_quadVao->bind();
        renderer.renderVertexArray(*m_quadVao);
        m_quadVao->unbind();

        m_colorBufferShader->disable();
        m_colorBuffer->unbind();
        m_bloomBuffer->unbind();
        renderer.restoreViewport();
    }

    void setColorBuffer(gfx::Texture* colorBuffer) {
        m_colorBuffer = colorBuffer;
    }

    void setBloomBuffer(gfx::Texture* bloomBuffer) {
        m_bloomBuffer = bloomBuffer;
    }

private:
    gfx::buffer::VertexArray* m_quadVao;
    gfx::Texture* m_colorBuffer;
    gfx::Texture* m_bloomBuffer;

    std::shared_ptr<gfx::Shader> m_colorBufferShader;
};

}