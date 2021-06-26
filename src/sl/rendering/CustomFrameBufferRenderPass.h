#pragma once

#include "RenderPass.h"
#include "StageBase.h"
#include "sl/gfx/LowLevelRenderer.h"
#include "sl/gfx/buffer/FrameBuffer.h"

namespace sl::rendering {

class CustomFrameBufferRenderPass : public RenderPass {
public:
    struct Stage : public StageBase {
        virtual void execute(gfx::LowLevelRenderer&, scene::Scene& scene, gfx::buffer::FrameBuffer&) = 0;
    };

    explicit CustomFrameBufferRenderPass(gfx::buffer::FrameBuffer* frameBuffer)
        : m_frameBuffer(frameBuffer) {
    }

    CustomFrameBufferRenderPass& addRenderStage(Stage* renderStage) {
        m_renderStages.push_back(renderStage);

        return *this;
    }

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene) {
        m_frameBuffer->bind();

        renderer.clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);

        for (auto& renderStage : m_renderStages)
            renderStage->execute(renderer, scene, *m_frameBuffer);

        m_frameBuffer->unbind();
    }

private:
    gfx::buffer::FrameBuffer* m_frameBuffer;
    std::vector<Stage*> m_renderStages;
};

}