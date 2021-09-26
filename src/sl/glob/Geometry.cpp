#include "Geometry.h"

#include "sl/geom/Mesh.h"
#include "sl/gfx/BufferManager.h"
#include "sl/gfx/ElementBuffer.h"
#include "sl/gfx/VertexBuffer.h"

namespace sl::glob {

Geometry::Geometry() {
    initSquares();
    initCube();
    initLine();

    meshes = {
        { upSquareMesh->name, upSquareMesh },
        { frontSquareMesh->name, frontSquareMesh },
        { cubeMesh->name, cubeMesh }
    };
}

void Geometry::initLine() {
    static std::vector<float> vertices = {
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f
    };

    lineVAO = gfx::BufferManager::get()->createVertexArray();
    auto vbo = gfx::BufferManager::get()->createVertexBuffer(&vertices[0], vertices.size() * sizeof(float), vertices.size());

    vbo->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

    lineVAO->addVertexBuffer(vbo);
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
    cubeVAO = gfx::BufferManager::get()->createVertexArray();
    auto vbo = gfx::BufferManager::get()->createVertexBuffer(&mesh->vertices[0], mesh->vertices.size() * sizeof(geom::Vertex), mesh->vertices.size());
    auto ebo = gfx::BufferManager::get()->createElementBuffer(&mesh->indices[0], mesh->indices.size() * sizeof(unsigned), mesh->indices.size());

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

    cubeMesh = mesh;
}

Geometry::MeshVaoPair Geometry::initSquare(std::vector<geom::Vertex> vertices, const std::string& name) {
    const std::vector<unsigned int> indices {
        0, 1, 2,
        0, 2, 3
    };

    auto mesh = std::make_shared<geom::Mesh>();

    mesh->indices = std::move(indices);
    mesh->vertices = std::move(vertices);

    // // TODO: move this logic to mesh contstructor
    auto vao = gfx::BufferManager::get()->createVertexArray();
    auto vbo = gfx::BufferManager::get()->createVertexBuffer(&mesh->vertices[0], mesh->vertices.size() * sizeof(geom::Vertex), mesh->vertices.size());
    auto ebo = gfx::BufferManager::get()->createElementBuffer(&mesh->indices[0], mesh->indices.size() * sizeof(unsigned), mesh->indices.size());

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

    vao->addVertexBuffer(vbo);
    vao->addElementBuffer(ebo);

    mesh->vertexArray = vao;
    mesh->name = name;

    return { mesh, vao };
}

void Geometry::initSquares() {
    const static math::Vec3 normal { 0.0f, 1.0f, 0.0f };

    const std::vector<geom::Vertex> verticesUp {
        geom::Vertex { { -1.0f, 0.0f, -1.0f }, normal, normal, normal, normal },
        geom::Vertex { { 1.0f, 0.0f, -1.0f }, normal, normal, normal, normal },
        geom::Vertex { { 1.0f, 0.0f, 1.0f }, normal, normal, normal, normal },
        geom::Vertex { { -1.0f, 0.0f, 1.0f }, normal, normal, normal, normal }
    };

    const std::vector<geom::Vertex> verticesFront {
        geom::Vertex { { -1.0, -1.0f, 0.0f }, normal, { 0.0f, 1.0f }, normal, normal },
        geom::Vertex { { 1.0f, -1.0f, 0.0f }, normal, { 1.0f, 1.0f }, normal, normal },
        geom::Vertex { { 1.0f, 1.0f, 0.0f }, normal, { 1.0f, 0.0f }, normal, normal },
        geom::Vertex { { -1.0f, 1.0f, 0.0f }, normal, { 0.0f, 0.0f }, normal, normal }
    };

    std::tie(upSquareMesh, upSquareVAO) = initSquare(std::move(verticesUp), "up-square");
    std::tie(frontSquareMesh, frontSquareVAO) = initSquare(std::move(verticesFront), "front-square");
}
}
