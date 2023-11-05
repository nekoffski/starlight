#pragma once

#include "Glm.h"

namespace sl {

struct Vertex3 {
    Vec3f position;
    Vec3f normal;
    Vec2f textureCoordinates;
    Vec4f color;
    Vec4f tangent;

    std::string toString() const;
};

}  // namespace sl
