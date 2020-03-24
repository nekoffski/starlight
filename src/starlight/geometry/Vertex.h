#pragma once

#include <starlight/math/Vector.hpp>

namespace starl::geometry {

struct Vertex {
    math::Vec3 position;
    math::Vec3 normal;
    math::Vec2 textureCoordinates;
    math::Vec3 tangent;
    math::Vec3 bitangent;
};
}