#include <starlight/rendering/renderer/ModelRenderer.h>

namespace starl::rendering::renderer {

void ModelRenderer::render() {
    for (const auto& [shader, modelRenderObjects] : m_shaderToModelRenderObjects) {
        shader->enable();
        shader->setUniform("projection", m_lowLevelRenderer.getProjectionMatrix());
        shader->setUniform("view", m_camera->getViewMatrix());

        for (const auto& modelRenderObject : modelRenderObjects) {
            shader->setUniform("model", *modelRenderObject->modelMatrix);
            renderModel(modelRenderObject->model);
        }

        shader->disable();
    }
}

void ModelRenderer::renderModel(const std::shared_ptr<geometry::Model>& model) {
    for (const auto& mesh : model->meshes) {
        renderMesh(mesh);
    }
}

void ModelRenderer::renderMesh(const std::shared_ptr<geometry::Mesh>& mesh) {
    int i = 0;
    for (const auto& texture : mesh->textures) {
        texture->bind(i++);
    }

    m_lowLevelRenderer.renderVertexArray(mesh->vertexArray);
    // is it neccessary?
    for (const auto& texture : mesh->textures) {
        texture->unbind();
    }
}
}
