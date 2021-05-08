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

    std::shared_ptr<gfx::buffer::VertexArray> getVertexArray() const override;
    std::vector<math::Vec3> getVertices() const override;
    std::string getName() const override;

    bool collide(Collider* collider, const math::Mat4& modelMatrix) override;
    Collider* getCollider() override;

    math::Vec3 min;
    math::Vec3 max;

private:
    void build(const std::vector<std::shared_ptr<geom::Mesh>> meshes);

    std::shared_ptr<gfx::buffer::VertexArray> m_vao;
    std::unique_ptr<AxisAlignedCollider> m_collider;

    std::vector<math::Vec3> m_vertices;
    std::vector<unsigned int> m_indices;
};

}