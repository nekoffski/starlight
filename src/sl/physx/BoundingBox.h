#pragma once

#include <vector>

#include "Collider.h"
#include "sl/gfx/VertexArray.h"
#include "sl/math/Vector.hpp"

namespace sl::physx {

struct BoundingBox {
    virtual gfx::VertexArray* getVertexArray() const = 0;
    virtual std::vector<math::Vec3> getVertices() const = 0;
    virtual std::string getName() const = 0;
    virtual const math::Vec3& getCenterOfMass() const = 0;

    virtual bool collide(Collider* collider, const math::Mat4& modelMatrix) = 0;
    virtual Collider* getCollider() = 0;
};

}