#pragma once

#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/math/Math.hh"
#include "starlight/renderer/core/RenderResource.hh"
#include "starlight/renderer/frontend/RenderScene.hh"

namespace sl {

struct RenderView {
    RenderTarget target;
};

struct RenderRequest {
    RenderScene scene;
    std::vector<RenderView> views;
    Vec4f clearColor{1.0f};
};

}  // namespace sl
