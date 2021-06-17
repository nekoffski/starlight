#pragma once

#include "RenderPass.h"

#include "sl/gfx/LowLevelRenderer.h"
#include "sl/scene/Scene.h"

namespace sl::rendering {

class RenderPipeline {
public:
    void run(sl::gfx::LowLevelRenderer& renderer, scene::Scene& scene) {
        bool hasSkybox = scene.skybox.has_value();

        if (hasSkybox)
            scene.skybox->cubemap->bind();

        scene.vectors.clear();

        for (auto& renderPass : m_renderPasses)
            renderPass->execute(renderer, scene);

        if (hasSkybox)
            scene.skybox->cubemap->unbind();
    }

    RenderPipeline& addRenderPass(RenderPass* renderPass) {
        m_renderPasses.push_back(renderPass);
        return *this;
    }

private:
    std::vector<RenderPass*> m_renderPasses;
};

}