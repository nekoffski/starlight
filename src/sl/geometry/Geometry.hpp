#pragma once

#include "Mesh.h"
#include "Model.h"

#include "sl/platform/gpu/ElementBuffer.h"
#include "sl/platform/gpu/VertexArray.h"
#include "sl/platform/gpu/VertexBuffer.h"

namespace sl::geometry {

class Geometry {
public:
    static void init() {
        initSquare();
    }

    static std::shared_ptr<Model> getSquare() {
        return m_square;
    }

    static std::shared_ptr<platform::gpu::VertexArray> getSquareVAO() {
        return m_squareVAO;
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
        m_squareVAO = platform::gpu::VertexArray::create();
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

        m_squareVAO->addVertexBuffer(vbo);
        m_squareVAO->addElementBuffer(ebo);
        mesh->vertexArray = m_squareVAO;

        m_square = std::make_shared<Model>();
        m_square->meshes.emplace_back(std::move(mesh));
    }

    inline static std::shared_ptr<platform::gpu::VertexArray> m_squareVAO;
    inline static std::shared_ptr<Model> m_square;
};
}