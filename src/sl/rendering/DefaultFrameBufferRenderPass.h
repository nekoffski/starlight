#pragma once

#include "RenderPass.h"
#include "StageBase.h"
#include "sl/gfx/LowLevelRenderer.h"

namespace sl::rendering {

class DefaultFrameBufferRenderPass : public RenderPass {
public:
    struct Stage : public StageBase {
        virtual void execute(gfx::LowLevelRenderer&, scene::Scene& scene) = 0;
    };

    DefaultFrameBufferRenderPass& addRenderStage(Stage* renderStage) {
        m_renderStages.push_back(renderStage);
        return *this;
    }

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene) {
        renderer.clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);

        for (auto& renderStage : m_renderStages)
            renderStage->execute(renderer, scene);
    }

private:
    std::vector<Stage*> m_renderStages;
};

}