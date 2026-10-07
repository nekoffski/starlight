#pragma once

#include "ShaderDataLayout.hh"
#include "starlight/core/Core.hh"
#include "starlight/math/Math.hh"

namespace sl {

struct Vertex3 {
    Vec3f position;
    // Vec3f normal;
    // Vec2f texCoord;

    static VertexLayout layout();
};

}  // namespace sl
