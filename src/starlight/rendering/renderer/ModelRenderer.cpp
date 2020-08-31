#include "ModelRenderer.h"

#include "starlight/core/log/Logger.h"
#include "starlight/math/Utils.hpp"
#include "starlight/platform/time/Clock.h"

namespace sl::rendering::renderer {

void ModelRenderer::render(std::shared_ptr<platform::shader::Shader> shader, const data::ModelData& modelData,
    const math::Mat4& transform) {

    float t = platform::time::Clock::now()->value();
    shader->setUniform("t", t);
    shader->setUniform("projection", m_lowLevelRenderer.getProjectionMatrix());

    for (const auto& position : modelData.positions) {
        shader->setUniform("model", transform * math::translate(position));
        renderModel(modelData.model);
    }
}

void ModelRenderer::renderModel(const std::shared_ptr<geometry::Model> model) {
    for (const auto& mesh : model->meshes)
        renderMesh(mesh);
}

void ModelRenderer::renderMesh(const std::shared_ptr<geometry::Mesh> mesh) {
    int i = 0;
    for (const auto& texture : mesh->textures)
        texture->bind(i++);

    m_lowLevelRenderer.renderVertexArray(mesh->vertexArray);
    // is it neccessary?
    for (const auto& texture : mesh->textures)
        texture->unbind();
}
}
