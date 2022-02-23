#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "Vertex.h"
#include "sl/core/GameObject.h"
#include "sl/gfx/BufferManager.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/VertexArray.h"

namespace sl::geom {

struct Mesh : core::GameObject {
    void buildVertexArray() {
        if (vertexArray != nullptr) vertexArray->unbind();

        auto vao = gfx::BufferManager::get().createVertexArray();
        auto vbo = gfx::BufferManager::get().createVertexBuffer(
            &vertices[0], vertices.size() * sizeof(Vertex), vertices.size());
        auto ebo = gfx::BufferManager::get().createElementBuffer(
            &indices[0], indices.size() * sizeof(unsigned), indices.size());

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

        vertexArray = vao;
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<sl::gfx::Texture>> textures;
    std::shared_ptr<gfx::VertexArray> vertexArray;

    std::string name = "ExampleMesh";
    std::optional<std::string> providedBy;
};

}  // namespace sl::geom
