#pragma once

#include "RenderPass.h"
#include "Stage.h"
#include "sl/gfx/Renderer.h"

namespace sl::rendering {

class DefaultFrameBufferRenderPass : public RenderPass {
public:
    DefaultFrameBufferRenderPass& addRenderStage(Stage* renderStage) {
        m_renderStages.push_back(renderStage);
        return *this;
    }

    void execute(gfx::Renderer& renderer, scene::Scene& scene) {
        renderer.clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);

        bool hasSkybox = scene.skybox.has_value();

        if (hasSkybox)
            scene.skybox->cubemap->bind();

        for (auto& renderStage : m_renderStages)
            renderStage->execute(renderer, scene, nullptr);

        if (hasSkybox)
            scene.skybox->cubemap->unbind();
    }

private:
    std::vector<Stage*> m_renderStages;
};

}