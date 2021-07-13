#pragma once

#include "sl/gfx/Shader.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/buffer/VertexArray.h"
#include "sl/rendering/Stage.h"
#include "sl/utils/Globals.h"

#include "sl/core/Logger.h"

namespace sl::rendering::stages {

class BlurColorBufferStage : public Stage {
public:
    enum class Direction {
        horizontal = 0,
        vertical
    };

    explicit BlurColorBufferStage()
        : m_quadVao(GLOBALS().geom->frontSquareVAO.get())
        , m_gaussianBlurShader(gfx::Shader::load(
              GLOBALS().config.paths.shaders + "/GaussianBlur.vert", GLOBALS().config.paths.shaders + "/GaussianBlur.frag"))
        , m_horizontalFrameBuffer(gfx::buffer::FrameBuffer::factory->create())
        , m_verticalFrameBuffer(gfx::buffer::FrameBuffer::factory->create()) {
    }

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene, gfx::buffer::FrameBuffer* frameBuffer) override {
        auto [width, height] = m_windowProxy->getSize();
        gfx::ViewFrustum::Viewport viewport { width, height };

        renderer.setTemporaryViewport(viewport);

        constexpr int passes = 10;

        bool horizontal = true;

        std::unordered_map<bool, std::pair<gfx::buffer::FrameBuffer*, gfx::Texture*>> buffers = {
            { true, { m_horizontalFrameBuffer.get(), m_verticalBuffer.get() } },
            { false, { m_verticalFrameBuffer.get(), m_horizontalBuffer.get() } }
        };

        for (int i = 0; i < passes; ++i) {
            auto [frameBuffer, colorBuffer] = buffers[horizontal];

            frameBuffer->bind();
            frameBuffer->specifyColorBuffers({ STARL_COLOR_ATTACHMENT0 });

            if (i != 0)
                colorBuffer->bind(0);
            else
                m_colorBuffer->bind(0);

            m_gaussianBlurShader->enable();
            m_gaussianBlurShader->setUniform("horizontal", horizontal);

            m_quadVao->bind();
            renderer.renderVertexArray(*m_quadVao);
            m_quadVao->unbind();

            frameBuffer->unbind();

            if (i != 0)
                colorBuffer->unbind();

            horizontal = !horizontal;
        }

        m_colorBuffer->unbind();
        m_gaussianBlurShader->disable();

        renderer.restoreViewport();
    }

    gfx::Texture* getOutputColorBuffer() const {
        return m_verticalBuffer.get();
    }

    void setColorBuffer(gfx::Texture* colorBuffer) {
        m_colorBuffer = colorBuffer;

        m_horizontalBuffer = colorBuffer->clone();
        m_verticalBuffer = colorBuffer->clone();

        m_verticalFrameBuffer->bind();
        m_verticalFrameBuffer->bindTexture(*m_verticalBuffer, STARL_COLOR_ATTACHMENT0);

        m_horizontalFrameBuffer->bind();
        m_horizontalFrameBuffer->bindTexture(*m_horizontalBuffer, STARL_COLOR_ATTACHMENT0);
        m_horizontalFrameBuffer->unbind();
    }

private:
    gfx::buffer::VertexArray* m_quadVao;

    gfx::Texture* m_colorBuffer;

    std::unique_ptr<gfx::Texture> m_horizontalBuffer;
    std::unique_ptr<gfx::Texture> m_verticalBuffer;

    std::shared_ptr<gfx::buffer::FrameBuffer> m_horizontalFrameBuffer;
    std::shared_ptr<gfx::buffer::FrameBuffer> m_verticalFrameBuffer;

    std::shared_ptr<gfx::Shader> m_gaussianBlurShader;

    Direction m_direction;
};

}