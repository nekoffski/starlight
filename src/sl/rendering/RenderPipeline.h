#pragma once

#include "RenderPass.h"

#include <kc/core/Profiler.h>
#include "sl/gfx/Renderer.h"
#include "sl/scene/Scene.h"

namespace sl::rendering {

class RenderPipeline {
public:
    void run(sl::gfx::Renderer& renderer, scene::Scene& scene) {
        PROFILE_FUNCTION();

        scene.vectors.clear();

        for (auto& renderPass : m_renderPasses)
            renderPass->execute(renderer, scene);
    }

    RenderPipeline& addRenderPass(RenderPass* renderPass) {
        m_renderPasses.push_back(renderPass);
        return *this;
    }

private:
    std::vector<RenderPass*> m_renderPasses;
};

}