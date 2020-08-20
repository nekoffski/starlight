#include "SceneManager.h"
#include "components/ModelComponent.h"
#include "components/RendererComponent.h"

namespace sl::scene {

SceneManager::SceneManager(std::shared_ptr<rendering::RendererProxy> renderer)
    : m_renderer(renderer) {
    m_rendererSystem = std::make_shared<systems::RendererSystem>(m_renderer);
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

void SceneManager::render(const std::shared_ptr<rendering::camera::Camera> camera) {
    const auto& skybox = m_scene->m_skybox;

    if (skybox)
        skybox->cubemap->bind();

    for (auto& rendererComponent : m_scene->m_ecsRegistry.getComponentView<components::RendererComponent>())
        m_rendererSystem->render(rendererComponent, camera);

    if (skybox) {
        skybox->cubemap->unbind();
        m_renderer->renderCubemap(skybox->cubemap, skybox->shader, camera);
    }
}

void SceneManager::renderSceneGUI(gui::Window& window) {
    for (const auto& [name, id] : m_scene->m_ecsRegistry.getEntityNameToId()) {
        window.displayText(std::move(name));
        if (window.isPreviousWidgetClicked())
            m_activeEntity = m_scene->m_ecsRegistry.getEntityById(std::move(id));
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
}