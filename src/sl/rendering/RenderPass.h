#pragma once

#include "sl/gpu/Renderer.h"
#include "sl/scene/Scene.h"

namespace sl::rendering {

struct RenderPass {
    virtual void execute(gpu::Renderer& renderer, scene::Scene& scene) = 0;
};

}  // namespace sl::rendering