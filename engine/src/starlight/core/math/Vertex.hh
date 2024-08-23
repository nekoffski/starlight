#pragma once

#include "Core.h"

namespace sl {

struct Vertex2 {
    Vec2<f32> position;
    Vec2<f32> textureCoordinates;
};

struct Vertex3 {
    Vec3<f32> position;
    Vec3<f32> normal;
    Vec2<f32> textureCoordinates;
    Vec4<f32> color;
    Vec4<f32> tangent;

    std::string toString() const;
};

}  // namespace sl
