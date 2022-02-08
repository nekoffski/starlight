#pragma once

#include <kc/core/Log.h>

#include "sl/core/WindowManager.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/ShaderManager.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/VertexArray.h"
#include "sl/glob/Globals.h"
#include "sl/rendering/Stage.h"

namespace sl::rendering::stages {

class RenderColorBufferStage : public Stage {
public:
    explicit RenderColorBufferStage()
        : m_quadVao(sl::glob::Globals::get().geom->frontSquareVAO.get())
        , m_colorBufferShader(gfx::ShaderManager::get().load(
              sl::glob::Globals::get().config.paths.shaders + "/ColorBuffer.vert",
              sl::glob::Globals::get().config.paths.shaders + "/ColorBuffer.frag")) {

// clang-format off
        #ifdef DEV_MODE
            async::AsyncManager::get().addPeriodicTask<gfx::Shader::RecompileOnUpdate>(m_colorBufferShader);
        #endif
        // clang-format on
    }

    void execute(gfx::Renderer& renderer, scene::Scene& scene, gfx::FrameBuffer*) override {
        auto [width, height] = core::WindowManager::get().getSize();
        gfx::ViewFrustum::Viewport viewport { width, height };

        auto settings = renderer.getSettings();
        settings.enableBlending = false;

        renderer.setTemporarySettings(settings);
        renderer.setTemporaryViewport(viewport);

        m_colorBufferShader->enable();

        m_colorBuffer->bind(1);
        m_bloomBuffer->bind(2);

        auto& visualSettings = sl::glob::Globals::get().visual;

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
        renderer.restoreSettings();
    }

    void setColorBuffer(gfx::Texture* colorBuffer) {
        m_colorBuffer = colorBuffer;
    }

    void setBloomBuffer(gfx::Texture* bloomBuffer) {
        m_bloomBuffer = bloomBuffer;
    }

private:
    gfx::VertexArray* m_quadVao;
    gfx::Texture* m_colorBuffer;
    gfx::Texture* m_bloomBuffer;

    std::shared_ptr<gfx::Shader> m_colorBufferShader;
};

}