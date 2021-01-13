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

void RendererSystem::render(components::RendererComponent& component, ecs::ComponentView<components::MaterialComponent> materials,
    ecs::ComponentView<components::ModelComponent> models, ecs::ComponentView<components::TransformComponent> transforms,
    std::shared_ptr<graphics::camera::Camera> camera, std::shared_ptr<graphics::Shader> shader) {

    shader->setUniform("view", camera->getViewMatrix());
    shader->setUniform("viewPos", camera->getPosition());

    auto& entityId = component.ownerEntityId;

    // TODO: POSSIBLE BRANCHING
    auto& material = materials.isEntityOwner(entityId) ? materials.getByEntityId(entityId) : DEFAULT_MATERIAL;

    setMaterial(material, shader);

    auto& model = models.getByEntityId(entityId);
    auto transformComponent = transforms.getByEntityId(entityId);

    renderModelComposite(shader, model.modelData, transformComponent());
}

void RendererSystem::render(components::RendererComponent& component, ecs::ComponentView<components::MaterialComponent> materials,
    ecs::ComponentView<components::ModelComponent> models, ecs::ComponentView<components::TransformComponent> transforms,
    std::shared_ptr<graphics::camera::Camera> camera) {

    render(component, materials, models, transforms, camera, component.shader);
}

void RendererSystem::setMaterial(const components::MaterialComponent& material, std::shared_ptr<graphics::Shader> shader) {
    shader->setUniform("material.ambientColor", material.ambientColor);
    shader->setUniform("material.diffuseColor", material.diffuseColor);
    shader->setUniform("material.specularColor", material.specularColor);
    shader->setUniform("material.shininess", material.shininess);
}

void RendererSystem::renderModelComposite(std::shared_ptr<graphics::Shader> shader, const graphics::data::ModelData& modelData,
    const math::Mat4& transform) {

    float t = core::Clock::now()->value();
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
