#pragma once

#include "RenderPass.h"
#include "Stage.h"
#include "sl/gfx/FrameBuffer.h"
#include "sl/gfx/Renderer.h"

namespace sl::rendering {

class CustomFrameBufferRenderPass : public RenderPass {
   public:
    explicit CustomFrameBufferRenderPass(gfx::FrameBuffer* frameBuffer)
        : m_frameBuffer(frameBuffer) {}

    CustomFrameBufferRenderPass& addRenderStage(Stage* renderStage) {
        m_renderStages.push_back(renderStage);

        return *this;
    }

    void execute(gfx::Renderer& renderer, scene::Scene& scene) {
        m_frameBuffer->bind();

        renderer.clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);

        for (auto& renderStage : m_renderStages)
            renderStage->execute(renderer, scene, m_frameBuffer);

        m_frameBuffer->unbind();
    }

   private:
    gfx::FrameBuffer* m_frameBuffer;
    std::vector<Stage*> m_renderStages;
};

}  // namespace sl::rendering