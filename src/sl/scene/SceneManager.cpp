#include "SceneManager.h"

#include "components/DirectionalLightComponent.h"
#include "components/ModelComponent.h"
#include "components/ParticleEffectComponent.h"
#include "components/RendererComponent.h"

namespace sl::scene {

SceneManager::SceneManager(std::shared_ptr<rendering::RendererProxy> renderer)
    : m_renderer(renderer)
    , m_rendererSystem(m_renderer)
    , m_pfxSystem(m_renderer) {
}

void SceneManager::update(float deltaTime) {
    m_pfxSystem.update(
        m_scene->m_ecsRegistry.getComponentView<components::ParticleEffectComponent>(), deltaTime);
}

void SceneManager::setActiveScene(std::shared_ptr<Scene> scene) {
    m_scene = std::move(scene);
}

void SceneManager::render(const std::shared_ptr<rendering::camera::Camera> camera) {
    const auto& skybox = m_scene->m_skybox;

    if (skybox)
        skybox->cubemap->bind();

    auto& directionalLights = m_scene->m_ecsRegistry.getComponentView<components::DirectionalLightComponent>();
    auto& pointLights = m_scene->m_ecsRegistry.getComponentView<components::PointLightComponent>();

    auto& rendererComponents = m_scene->m_ecsRegistry.getComponentView<components::RendererComponent>();

    // m_renderer->beginDepthCapture();
    // for (auto& rendererComponent : rendererComponents)
    //     m_rendererSystem.render(rendererComponent, camera);
    // m_renderer->endDepthCapture();

    for (auto& rendererComponent : rendererComponents) {
        rendererComponent.shader->enable();
        m_lightSystem.prepareDirectionalLights(directionalLights, rendererComponent.shader);
        m_lightSystem.preparePointsLights(pointLights, rendererComponent.shader);
        m_rendererSystem.render(rendererComponent, camera);
        rendererComponent.shader->disable();
    }

    m_pfxSystem.renderParticleEffects(
        m_scene->m_ecsRegistry.getComponentView<components::ParticleEffectComponent>(), camera);

    if (skybox) {
        m_renderer->renderCubemap(skybox->cubemap, skybox->shader, camera);
        skybox->cubemap->unbind();
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