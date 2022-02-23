#pragma once

#include "sl/math/Vector.hpp"

namespace sl::geom {

struct Vertex {
    math::Vec3 position;
    math::Vec3 normal;
    math::Vec2 textureCoordinates;
    math::Vec3 tangent;
    math::Vec3 bitangent;
};
}  // namespace sl::geom