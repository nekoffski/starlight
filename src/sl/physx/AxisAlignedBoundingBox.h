#pragma once

#include <memory>
#include <vector>

#include "BoundingBox.h"
#include "geom/Mesh.h"

namespace sl::physx {

class AxisAlignedBoundingBox : public BoundingBox {
public:
    explicit AxisAlignedBoundingBox(const std::vector<std::shared_ptr<geom::Mesh>> meshes)
        : m_vertices(findVertices(meshes)) {
    }

    std::shared_ptr<gfx::buffer::VertexArray> getVertexArray() const override {
        return nullptr;
    }

    std::vector<geom::Vertex> getVertices() const override {
        return m_vertices;
    }

private:
    std::vector<geom::Vertex> findVertices(const std::vector<std::shared_ptr<geom::Mesh>> meshes) {
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

        return std::vector<geom::Vertex> {
            geom::Vertex { { min.x, min.y, min.z } },
            geom::Vertex { { max.x, min.y, min.z } },
            geom::Vertex { { min.x, min.y, max.z } },
            geom::Vertex { { max.x, min.y, max.z } },
            geom::Vertex { { min.x, max.y, min.z } },
            geom::Vertex { { max.x, max.y, min.z } },
            geom::Vertex { { min.x, max.y, max.z } },
            geom::Vertex { { max.x, max.y, max.z } }

        };
    }

    std::vector<geom::Vertex> m_vertices;
};

}