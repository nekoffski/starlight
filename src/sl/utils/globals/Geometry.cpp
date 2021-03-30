#include "Geometry.h"

#include "sl/geom/Mesh.h"
#include "sl/gfx/buffer/ElementBuffer.h"
#include "sl/gfx/buffer/VertexBuffer.h"

namespace sl::utils::globals {

Geometry::Geometry() {
    initSquare();
}

void Geometry::initSquare() {
    const static std::vector<unsigned int> indices {
        0, 1, 2,
        0, 2, 3
    };

    const static math::Vec3 normal { 0.0f, 1.0f, 0.0f };

    const static std::vector<geom::Vertex> vertices {
        geom::Vertex { { -0.5f, 0.0f, -0.5f }, normal, normal, normal, normal },
        geom::Vertex { { 0.5f, 0.0f, -0.5f }, normal, normal, normal, normal },
        geom::Vertex { { 0.5f, 0.0f, 0.5f }, normal, normal, normal, normal },
        geom::Vertex { { -0.5f, 0.0f, 0.5f }, normal, normal, normal, normal }
    };

    auto mesh = std::make_shared<geom::Mesh>();

    mesh->indices = std::move(indices);
    mesh->vertices = std::move(vertices);

    // TODO: move this logic to mesh contstructor
    squareVAO = gfx::buffer::VertexArray::factory->create();
    auto vbo = gfx::buffer::VertexBuffer::factory->create(&mesh->vertices[0], mesh->vertices.size() * sizeof(geom::Vertex), mesh->vertices.size());
    auto ebo = gfx::buffer::ElementBuffer::factory->create(&mesh->indices[0], mesh->indices.size() * sizeof(unsigned), mesh->indices.size());

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

    squareModel = std::make_shared<geom::Model>();
    squareModel->meshes.emplace_back(std::move(mesh));
}
}
