#pragma once

#include "sl/geom/Mesh.h"
#include "sl/gfx/Renderer.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::rendering::utils {

inline void renderMesh(gfx::Renderer& renderer, geom::Mesh& mesh) {
    int i = 0;
    for (const auto& texture : mesh.textures) texture->bind(i++);

    auto& vao = mesh.vertexArray;

    vao->bind();
    renderer.renderVertexArray(*vao);
    vao->unbind();

    for (const auto& texture : mesh.textures) texture->unbind();
}

inline void renderMeshWithoutTextures(gfx::Renderer& renderer, geom::Mesh& mesh) {
    auto& vao = mesh.vertexArray;

    vao->bind();
    renderer.renderVertexArray(*vao);
    vao->unbind();
}

inline void renderModel(
    gfx::Renderer& renderer, gfx::Shader& shader, scene::components::ModelComponent& model,
    const math::Mat4& transform
) {
    for (auto& position : model.instances) {
        shader.setUniform("modelMatrix", transform * math::translate(position));

        for (auto& mesh : model.meshes) {
            shader.setUniform("textures", static_cast<unsigned int>(mesh->textures.size()));
            renderMesh(renderer, *mesh);
        }
    }
}

}  // namespace sl::rendering::utils