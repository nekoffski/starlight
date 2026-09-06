#pragma once

#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/math/Math.hh"

namespace sl {

struct RenderView {
    RenderOutput output;
};

struct RenderRequest {
    std::vector<RenderView> views;
    Vec4f clearColor{1.0f};
};

}  // namespace sl
