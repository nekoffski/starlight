#include "SceneManager.h"
#include "components/ModelComponent.h"

namespace sl::scene {

SceneManager::SceneManager(std::shared_ptr<rendering::RendererProxy> renderer)
    : m_renderer(std::move(renderer)) {
}

void SceneManager::update(float deltaTime) {
    // m_modelSystem->clean();

    // for (auto& model : m_scene->m_ecsRegistry.getComponentView<component::ModelComponent>())
    // m_modelSystem->processComponentImpl(model, nullptr);

    // for (auto& entity : m_scene->m_entities)
    // processEntity(entity);
}

void SceneManager::setActiveScene(std::shared_ptr<Scene> scene) {
    m_scene = std::move(scene);
}

void SceneManager::render(const std::shared_ptr<rendering::camera::Camera>& camera) {
    const auto& skybox = m_scene->m_skybox;

    if (skybox)
        skybox->cubemap->bind();

    // m_renderer->renderModels(m_modelSystem->getModels(), camera);

    if (skybox) {
        skybox->cubemap->unbind();
        m_renderer->renderCubemap(skybox->cubemap, skybox->shader, camera);
    }
}

void SceneManager::renderSceneGUI(gui::Window& window) {
    for (auto& entity : m_scene->m_entities) {
        window.displayText(entity->getName());
        if (window.isPreviousWidgetClicked())
            m_activeEntity = entity;
    }
}

void SceneManager::renderMainGUI(gui::Window& window) {
    if (auto entity = m_activeEntity.lock()) {
        if (window.beginTreeNode("Selected entity")) {
            entity->onGUI(window);
            window.popTreeNode();
        }
    }
}

// void SceneManager::processEntity(std::shared_ptr<ecs::entity::Entity> entity) {
// for (auto& model : m_scene->m_ecsRegistry.getComponentView<ecs::component::ModelComponent>()) {
// m_modelSystem->processComponentImpl(model, entity);
// }
// m_scene->m_ecsRegistry.forEach<ecs::component::ModelComponent>([this, &entity](ecs::component::ModelComponent& component) {
// this->m_modelSystem->processComponentImpl(component, entity);
// });
// for (auto& component : entity->getComponents())
//     if (const auto type = component->getType(); m_systems.count(type) > 0)
//         m_systems.at(type)->processComponent(component, entity);

// for (auto& child : entity->getChilds())
// processEntity(child);
// }
}