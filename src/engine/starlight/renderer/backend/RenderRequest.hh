#pragma once

#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/math/Math.hh"
#include "starlight/renderer/rhi/RenderResource.hh"

namespace sl {

struct RenderView {
    RenderTarget target;
};

struct RenderRequest {
    std::vector<RenderView> views;
    Vec4f clearColor{1.0f};
};

}  // namespace sl
