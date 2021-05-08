#pragma once

#include <memory>
#include <vector>

#include "sl/core/Logger.h"

#include "BoundingBox.h"
#include "sl/geom/Mesh.h"

namespace sl::physx {

class AxisAlignedBoundingBox : public BoundingBox {
public:
    explicit AxisAlignedBoundingBox(const std::vector<std::shared_ptr<geom::Mesh>> meshes)
        : m_vao(nullptr) {
        build(meshes);
    }

    std::shared_ptr<gfx::buffer::VertexArray> getVertexArray() const override {
        return m_vao;
    }

    std::vector<math::Vec3> getVertices() const override {
        return m_vertices;
    }

private:
    void build(const std::vector<std::shared_ptr<geom::Mesh>> meshes) {
        SL_ASSERT(meshes.size() > 0, "Cannot build bounding box for empty meshes vector");

        math::Vec3 min = meshes[0]->vertices[0].position;
        math::Vec3 max = min;

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
    }

    std::shared_ptr<gfx::buffer::VertexArray> m_vao;

    std::vector<math::Vec3> m_vertices;
    std::vector<unsigned int> m_indices;
};

}