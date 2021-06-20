#include "AxisAlignedBoundingBox.h"

#include "AxisAlignedCollider.h"

#include "sl/core/Logger.h"

namespace sl::physx {

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const std::vector<std::shared_ptr<geom::Mesh>> meshes)
    : m_vao(nullptr) {
    build(meshes);
}

gfx::buffer::VertexArray* AxisAlignedBoundingBox::getVertexArray() const {
    return m_vao.get();
}

std::vector<math::Vec3> AxisAlignedBoundingBox::getVertices() const {
    return m_vertices;
}

std::string AxisAlignedBoundingBox::getName() const {
    return "AABB";
}

void AxisAlignedBoundingBox::build(const std::vector<std::shared_ptr<geom::Mesh>> meshes) {
    SL_ASSERT(meshes.size() > 0, "Cannot build bounding box for empty meshes vector");

    min = meshes[0]->vertices[0].position;
    max = min;

    for (const auto& mesh : meshes) {
        for (const auto& vertex : mesh->vertices) {
            auto& position = vertex.position;

            if (min.z > position.z)
                min.z = position.z;
            if (min.y > position.y)
                min.y = position.y;
            if (min.x > position.x)
                min.x = position.x;

            if (max.z < position.z)
                max.z = position.z;
            if (max.y < position.y)
                max.y = position.y;
            if (max.x < position.x)
                max.x = position.x;
        }
    }

    m_vertices = std::vector<math::Vec3> {
        // front
        { min.x, min.y, max.z },
        { max.x, min.y, max.z },
        { max.x, max.y, max.z },
        { min.x, max.y, max.z },
        // back
        { min.x, min.y, min.z },
        { max.x, min.y, min.z },
        { max.x, max.y, min.z },
        { min.x, max.y, min.z },
    };

    m_indices = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };

    m_vao = gfx::buffer::VertexArray::factory->create();
    auto vbo = gfx::buffer::VertexBuffer::factory->create(&m_vertices[0], m_vertices.size() * sizeof(math::Vec3), m_vertices.size());
    auto ebo = gfx::buffer::ElementBuffer::factory->create(&m_indices[0], m_indices.size() * sizeof(unsigned), m_indices.size());

    vbo->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

    m_vao->addVertexBuffer(vbo);
    m_vao->addElementBuffer(ebo);

    m_collider = std::make_unique<AxisAlignedCollider>(min, max);

    m_centerOfMass = {
        (min.x + max.x) / 2.0f,
        (min.y + max.z) / 2.0f,
        (min.z + max.z) / 2.0f
    };
}

bool AxisAlignedBoundingBox::collide(Collider* collider, const math::Mat4& modelMatrix) {
    return collider->collideWith(*this, modelMatrix);
}

Collider* AxisAlignedBoundingBox::getCollider() {
    return m_collider.get();
}

const math::Vec3& AxisAlignedBoundingBox::getCenterOfMass() const {
    return m_centerOfMass;
}

}