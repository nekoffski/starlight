#include "RendererSystem.h"

#include "starlight/core/perf/Profiler.hpp"
#include "starlight/rendering/RendererProxy.h"
#include "starlight/scene/components/MaterialComponent.h"
#include "starlight/scene/components/ModelComponent.h"
#include "starlight/scene/components/RendererComponent.h"
#include "starlight/scene/components/TransformComponent.h"

namespace sl::scene::systems {

const components::MaterialComponent DEFAULT_MATERIAL = components::MaterialComponent();

RendererSystem::RendererSystem(std::shared_ptr<rendering::RendererProxy> renderer)
    : m_renderer(renderer) {
}

void RendererSystem::render(components::RendererComponent& component, std::shared_ptr<rendering::camera::Camera> camera) {
    auto& entity = component.entity;
    auto& shader = component.shader;

    shader->setUniform("view", camera->getViewMatrix());
    shader->setUniform("viewPos", camera->getPosition());

    // POSSIBLE BRANCHING
    auto& material = entity->hasComponent<components::MaterialComponent>()
        ? entity->getComponent<components::MaterialComponent>()
        : DEFAULT_MATERIAL;

    setMaterial(material, shader);

    auto& model = entity->getComponent<components::ModelComponent>();
    auto transformComponent = entity->getComponent<components::TransformComponent>();

    m_renderer->renderModels(shader, model.modelData, transformComponent());
}

void RendererSystem::setMaterial(const components::MaterialComponent& material, std::shared_ptr<platform::shader::Shader> shader) {
    shader->setUniform("material.ambientColor", material.ambientColor);
    shader->setUniform("material.diffuseColor", material.diffuseColor);
    shader->setUniform("material.specularColor", material.specularColor);
    shader->setUniform("material.shininess", material.shininess);
}
}
