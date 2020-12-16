#include "RendererSystem.h"

#include "sl/core/Profiler.h"
#include "sl/graphics/Renderer.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/RendererComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::scene::systems {

const components::MaterialComponent DEFAULT_MATERIAL = components::MaterialComponent();

RendererSystem::RendererSystem(std::shared_ptr<graphics::Renderer> renderer)
    : m_renderer(renderer) {
}

void RendererSystem::render(components::RendererComponent& component, std::shared_ptr<graphics::camera::Camera> camera,
    std::shared_ptr<platform::shader::Shader> shader) {
    auto& entity = component.entity;

    shader->setUniform("view", camera->getViewMatrix());
    shader->setUniform("viewPos", camera->getPosition());

    // POSSIBLE BRANCHING
    auto& material = entity->hasComponent<components::MaterialComponent>()
        ? entity->getComponent<components::MaterialComponent>()
        : DEFAULT_MATERIAL;

    setMaterial(material, shader);

    auto& model = entity->getComponent<components::ModelComponent>();
    auto transformComponent = entity->getComponent<components::TransformComponent>();

    renderModelComposite(shader, model.modelData, transformComponent());
}

void RendererSystem::render(components::RendererComponent& component, std::shared_ptr<graphics::camera::Camera> camera) {
    render(component, camera, component.shader);
}

void RendererSystem::setMaterial(const components::MaterialComponent& material, std::shared_ptr<platform::shader::Shader> shader) {
    shader->setUniform("material.ambientColor", material.ambientColor);
    shader->setUniform("material.diffuseColor", material.diffuseColor);
    shader->setUniform("material.specularColor", material.specularColor);
    shader->setUniform("material.shininess", material.shininess);
}

void RendererSystem::renderModelComposite(std::shared_ptr<platform::shader::Shader> shader, const graphics::data::ModelData& modelData,
    const math::Mat4& transform) {

    float t = platform::time::Clock::now()->value();
    shader->setUniform("t", t);
    shader->setUniform("projection", m_renderer->getProjectionMatrix());

    for (const auto& position : modelData.positions) {
        shader->setUniform("model", transform * math::translate(position));
        renderModel(modelData.model);
    }
}

void RendererSystem::renderModel(std::shared_ptr<geometry::Model> model) {
    for (const auto& mesh : model->meshes)
        renderMesh(mesh);
}

void RendererSystem::renderMesh(std::shared_ptr<geometry::Mesh> mesh) {
    int i = 0;
    for (const auto& texture : mesh->textures)
        texture->bind(i++);

    auto& vao = mesh->vertexArray;

    vao->bind();
    m_renderer->renderVertexArray(vao);
    vao->unbind();

    // is it neccessary?
    for (const auto& texture : mesh->textures)
        texture->unbind();
}
}
