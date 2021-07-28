#pragma once

#include <memory>
#include <vector>

#include "BoundingBox.h"
#include "fwd.h"
#include "sl/geom/Mesh.h"

#include "AxisAlignedCollider.h"

namespace sl::physx {

class AxisAlignedBoundingBox : public BoundingBox {
public:
    explicit AxisAlignedBoundingBox(const std::vector<std::shared_ptr<geom::Mesh>> meshes);

    gfx::VertexArray* getVertexArray() const override;
    std::vector<math::Vec3> getVertices() const override;
    std::string getName() const override;

    bool collide(Collider* collider, const math::Mat4& modelMatrix) override;
    Collider* getCollider() override;

    const math::Vec3& getCenterOfMass() const override;

    math::Vec3 min;
    math::Vec3 max;

private:
    void build(const std::vector<std::shared_ptr<geom::Mesh>> meshes);

    std::shared_ptr<gfx::VertexArray> m_vao;
    std::unique_ptr<AxisAlignedCollider> m_collider;

    std::vector<math::Vec3> m_vertices;
    std::vector<unsigned int> m_indices;

    math::Vec3 m_centerOfMass;
};

}