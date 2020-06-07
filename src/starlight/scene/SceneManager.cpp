#include <starlight/scene/SceneManager.h>

namespace starl::scene {

SceneManager::SceneManager(std::shared_ptr<rendering::RendererProxy> renderer)
    : m_renderer(std::move(renderer)) {
    m_modelSystem = std::make_shared<ecs::system::ModelSystem>();
    m_transformSystem = std::make_shared<ecs::system::TransformSystem>();
    m_pfxSystem = std::make_shared<ecs::system::PFXSystem>();

    m_systems.insert({ ecs::component::ComponentType::MODEL, m_modelSystem });
    m_systems.insert({ ecs::component::ComponentType::TRANSFORM, m_transformSystem });
    m_systems.insert({ ecs::component::ComponentType::PFX, m_pfxSystem });
}

void SceneManager::update(float deltaTime) {
    m_modelSystem->clean();

    for (auto& entity : m_scene->m_entities)
        processEntity(entity);
}

void SceneManager::setActiveScene(std::shared_ptr<Scene> scene) {
    m_scene = std::move(scene);
}

void SceneManager::render(const std::shared_ptr<framework::graphics::camera::Camera>& camera) {
    m_renderer->renderModels(m_modelSystem->getModels(), camera);

    if (const auto& skybox = m_scene->m_skybox; skybox)
        m_renderer->renderCubemap(skybox->cubemap, skybox->shader, camera);
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

void SceneManager::processEntity(std::shared_ptr<ecs::entity::Entity>& entity) {
    for (auto& component : entity->getComponents())
        if (const auto type = component->getType(); m_systems.count(type) > 0)
            m_systems.at(type)->processComponent(component, entity);

    for (auto& child : entity->getChilds())
        processEntity(child);
}
}