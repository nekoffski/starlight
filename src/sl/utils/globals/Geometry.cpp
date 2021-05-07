#include "Geometry.h"

#include "sl/geom/Mesh.h"
#include "sl/gfx/buffer/ElementBuffer.h"
#include "sl/gfx/buffer/VertexBuffer.h"

namespace sl::utils::globals {

Geometry::Geometry() {
    initSquare();
    initCube();

    meshes = {
        { squareMesh->name, squareMesh },
        { cubeMesh->name, cubeMesh }
    };
}

void Geometry::initCube() {
    const static math::Vec3 normal { 0.0f, 1.0f, 0.0f };

    const static std::vector<geom::Vertex> vertices {
        // front
        geom::Vertex { { -1.0, -1.0, 1.0 }, normal, normal, normal, normal },
        geom::Vertex { { 1.0, -1.0, 1.0 }, normal, normal, normal, normal },
        geom::Vertex { { 1.0, 1.0, 1.0 }, normal, normal, normal, normal },
        geom::Vertex { { -1.0, 1.0, 1.0 }, normal, normal, normal, normal },
        // back
        geom::Vertex { { -1.0, -1.0, -1.0 }, normal, normal, normal, normal },
        geom::Vertex { { 1.0, -1.0, -1.0 }, normal, normal, normal, normal },
        geom::Vertex { { 1.0, 1.0, -1.0 }, normal, normal, normal, normal },
        geom::Vertex { { -1.0, 1.0, -1.0 }, normal, normal, normal, normal }
    };

    const static std::vector<unsigned int> indices {
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

    auto mesh = std::make_shared<geom::Mesh>();

    mesh->indices = std::move(indices);
    mesh->vertices = std::move(vertices);

    // TODO: move this logic to mesh contstructor
    cubeVAO = gfx::buffer::VertexArray::factory->create();
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

    cubeVAO->addVertexBuffer(vbo);
    cubeVAO->addElementBuffer(ebo);

    mesh->vertexArray = cubeVAO;
    mesh->name = "Cube";

    // TODO: fixme
    mesh->setId(11);

    cubeMesh = mesh;
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
    mesh->name = "Square";

    squareMesh = mesh;
}
}
