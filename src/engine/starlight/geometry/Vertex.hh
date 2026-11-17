#pragma once

#include "starlight/core/Core.hh"
#include "starlight/math/Math.hh"
#include "starlight/renderer/core/ShaderDataLayout.hh"

namespace sl {

struct Vertex3 {
    Vec3f position;
    Vec3f normal;
    Vec2f texCoord;

    static VertexLayout layout();
};

}  // namespace sl
