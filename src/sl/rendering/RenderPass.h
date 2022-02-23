#pragma once

#include "sl/gfx/Renderer.h"
#include "sl/scene/Scene.h"

namespace sl::rendering {

struct RenderPass {
    virtual void execute(gfx::Renderer& renderer, scene::Scene& scene) = 0;
};

}  // namespace sl::rendering