#pragma once

#include "sl/gfx/LowLevelRenderer.h"
#include "sl/scene/Scene.h"

namespace sl::rendering {

struct RenderPass {
    virtual void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene) = 0;
};

}