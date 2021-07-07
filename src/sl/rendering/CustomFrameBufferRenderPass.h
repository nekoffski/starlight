#pragma once

#include "RenderPass.h"
#include "Stage.h"
#include "sl/gfx/LowLevelRenderer.h"
#include "sl/gfx/buffer/FrameBuffer.h"

namespace sl::rendering {

class CustomFrameBufferRenderPass : public RenderPass {
public:
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
            renderStage->execute(renderer, scene, m_frameBuffer);

        m_frameBuffer->unbind();
    }

private:
    gfx::buffer::FrameBuffer* m_frameBuffer;
    std::vector<Stage*> m_renderStages;
};

}