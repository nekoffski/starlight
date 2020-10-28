#include "SceneManager.h"

#include "components/DirectionalLightComponent.h"
#include "components/ModelComponent.h"
#include "components/ParticleEffectComponent.h"
#include "components/RendererComponent.h"
#include "sl/platform/texture/Texture.h"

#include <imgui/imgui.h>

#include <memory>

namespace sl::scene {

SceneManager::SceneManager(std::shared_ptr<rendering::Renderer> renderer)
    : m_renderer(renderer)
    , m_rendererSystem(m_renderer)
    , m_pfxSystem(m_renderer)
    , m_skyboxSystem(m_renderer) {}

void SceneManager::update(float deltaTime) {
    m_pfxSystem.update(
        m_scene->m_ecsRegistry.getComponentView<components::ParticleEffectComponent>(), deltaTime);
}

void SceneManager::setActiveScene(std::shared_ptr<Scene> scene) {
    m_scene = std::move(scene);
}

void SceneManager::render(std::shared_ptr<rendering::camera::Camera> camera) {
    const auto& skybox = m_scene->m_skybox;

    if (skybox)
        skybox->cubemap->bind();

    auto& directionalLights = m_scene->m_ecsRegistry.getComponentView<components::DirectionalLightComponent>();
    auto& pointLights = m_scene->m_ecsRegistry.getComponentView<components::PointLightComponent>();

    auto& rendererComponents = m_scene->m_ecsRegistry.getComponentView<components::RendererComponent>();
    // TO REFACTOR

    /*auto lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);*/
    //glm::mat4 lightView;
    //glm::mat4 lightSpace;

    /*m_renderer->beginDepthCapture();*/
    //for (auto& directionalLight : directionalLights) {
    //lightView = glm::lookAt(directionalLight.direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    //m_renderer->setShadowMap(directionalLight.shadowMap);
    //for (auto& rendererComponent : rendererComponents) {
    //auto tmp = rendererComponent.shader;
    //auto depthShader = m_renderer->getDepthShader();
    //lightSpace = lightProjection * lightView;
    //depthShader->setUniform("lightSpaceMatrix", lightProjection * lightView);
    //rendererComponent.shader = depthShader;
    //m_rendererSystem.render(rendererComponent, camera);
    //rendererComponent.shader = tmp;
    //}
    //}

    ////  m_renderer->restoreViewport();
    //m_renderer->endDepthCapture();

    for (auto& rendererComponent : rendererComponents) {
        rendererComponent.shader->enable();
        // rendererComponent.shader->setUniform("lightSpaceMatrix", lightSpace);
        m_lightSystem.prepareDirectionalLights(directionalLights, rendererComponent.shader);
        m_lightSystem.preparePointsLights(pointLights, rendererComponent.shader);
        m_rendererSystem.render(rendererComponent, camera);
        rendererComponent.shader->disable();
    }

    m_pfxSystem.renderParticleEffects(
        m_scene->m_ecsRegistry.getComponentView<components::ParticleEffectComponent>(), camera);

    if (skybox) {
        m_skyboxSystem.render(skybox->cubemap, skybox->shader, camera);
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
