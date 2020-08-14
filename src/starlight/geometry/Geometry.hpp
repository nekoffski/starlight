#pragma once

#include "Mesh.h"
#include "Model.h"

#include "starlight/platform/gpu/ElementBuffer.h"
#include "starlight/platform/gpu/VertexArray.h"
#include "starlight/platform/gpu/VertexBuffer.h"

namespace sl::geometry {

class Geometry {
public:
    static void init() {
        initSquare();
    }

    static std::shared_ptr<Model> getSquare() {
        return m_square;
    }

private:
    static void initSquare() {

        std::vector<unsigned int> indices{
            0, 1, 2,
            0, 2, 3
        };

        auto normal = math::Vec3{ 0.0f, 1.0f, 0.0f };

        std::vector<Vertex> vertices{
            Vertex{ { -0.5f, 0.0f, -0.5f }, normal, normal, normal, normal },
            Vertex{ { 0.5f, 0.0f, -0.5f }, normal, normal, normal, normal },
            Vertex{ { 0.5f, 0.0f, 0.5f }, normal, normal, normal, normal },
            Vertex{ { -0.5f, 0.0f, 0.5f }, normal, normal, normal, normal }
        };

        auto mesh = std::make_shared<Mesh>();

        mesh->indices = std::move(indices);
        mesh->vertices = std::move(vertices);

        // TODO: move this logic to mesh contstructor
        auto vao = platform::gpu::VertexArray::create();
        auto vbo = platform::gpu::VertexBuffer::create(&mesh->vertices[0], mesh->vertices.size() * sizeof(Vertex), mesh->vertices.size());
        auto ebo = platform::gpu::ElementBuffer::create(&mesh->indices[0], mesh->indices.size() * sizeof(unsigned), mesh->indices.size());

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

        m_square = std::make_shared<Model>();
        m_square->meshes.emplace_back(std::move(mesh));
    }

    inline static std::shared_ptr<Model> m_square;
};
}