#include "ModelRenderer.h"

#include "starlight/core/log/Logger.h"

namespace sl::rendering::renderer {

void ModelRenderer::render(const ShaderToModelRenderData& shaderToModelRenderData,
    const std::shared_ptr<camera::Camera> camera) {
    for (const auto& [shader, modelRenderDatas] : shaderToModelRenderData) {
        shader->enable();
        shader->setUniform("projection", m_lowLevelRenderer.getProjectionMatrix());
        shader->setUniform("view", camera->getViewMatrix());
        shader->setUniform("cameraPosition", camera->getPosition());

        float t = static_cast<float>(std::clock()) / CLOCKS_PER_SEC;
        shader->setUniform("t", t);

        for (const auto& modelRenderData : modelRenderDatas) {
            for (const auto& modelMatrix : modelRenderData.modelMatrices) {
                shader->setUniform("model", modelMatrix);
                renderModel(modelRenderData.model);
            }
        }

        shader->disable();
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
