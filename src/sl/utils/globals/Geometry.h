#pragma once

#include "sl/geometry/Mesh.h"
#include "sl/geometry/Model.h"

#include "sl/graphics/buffer/ElementBuffer.h"
#include "sl/graphics/buffer/VertexArray.h"
#include "sl/graphics/buffer/VertexBuffer.h"

namespace sl::utils::globals {

class Geometry {
public:
    explicit Geometry() {
        initSquare();
    }

    std::shared_ptr<graphics::buffer::VertexArray> squareVAO;
    std::shared_ptr<geometry::Model> squareModel;

private:
    void initSquare() {
        std::vector<unsigned int> indices{
            0, 1, 2,
            0, 2, 3
        };

        auto normal = math::Vec3{ 0.0f, 1.0f, 0.0f };

        std::vector<geometry::Vertex> vertices{
            geometry::Vertex{ { -0.5f, 0.0f, -0.5f }, normal, normal, normal, normal },
            geometry::Vertex{ { 0.5f, 0.0f, -0.5f }, normal, normal, normal, normal },
            geometry::Vertex{ { 0.5f, 0.0f, 0.5f }, normal, normal, normal, normal },
            geometry::Vertex{ { -0.5f, 0.0f, 0.5f }, normal, normal, normal, normal }
        };

        auto mesh = std::make_shared<geometry::Mesh>();

        mesh->indices = std::move(indices);
        mesh->vertices = std::move(vertices);

        // TODO: move this logic to mesh contstructor
        squareVAO = graphics::buffer::VertexArray::factory->create();
        auto vbo = graphics::buffer::VertexBuffer::factory->create(&mesh->vertices[0], mesh->vertices.size() * sizeof(geometry::Vertex), mesh->vertices.size());
        auto ebo = graphics::buffer::ElementBuffer::factory->create(&mesh->indices[0], mesh->indices.size() * sizeof(unsigned), mesh->indices.size());

        // vertices
        vbo->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));
        // normals
        vbo->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));
        // tex coords
        vbo->addMemoryOffsetScheme(2, STARL_FLOAT, sizeof(float));
        // tangents
        vbo->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));
        // bitangents
        vbo->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

        squareVAO->addVertexBuffer(vbo);
        squareVAO->addElementBuffer(ebo);
        mesh->vertexArray = squareVAO;

        squareModel = std::make_shared<geometry::Model>();
        squareModel->meshes.emplace_back(std::move(mesh));
    }
};
}
