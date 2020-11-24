#include "SceneManager3D.h"

#include <exception>

#include "components/DirectionalLightComponent.h"
#include "components/ModelComponent.h"
#include "components/ParticleEffectComponent.h"
#include "components/RendererComponent.h"
#include "sl/platform/texture/Texture.h"

#include <imgui/imgui.h>

#include <memory>

namespace sl::scene {

SceneManager3D::SceneManager3D(std::shared_ptr<rendering::Renderer> renderer)
    : m_renderer(renderer)
    , m_rendererSystem(m_renderer)
    , m_pfxSystem(m_renderer)
    , m_skyboxSystem(m_renderer)
    , m_shadowSystem(renderer) {}

void SceneManager3D::update(float deltaTime) {
    auto& pfxs = m_scene->m_ecsRegistry.getComponentView<components::ParticleEffectComponent>();
    m_pfxSystem.update(pfxs, deltaTime, m_scene->m_camera);
}

void SceneManager3D::setActiveScene(std::shared_ptr<Scene> scene) {
    auto scene3d = std::dynamic_pointer_cast<Scene3D>(scene);

	SL_ASSERT(scene3d != nullptr, "Invalid scene type!");
    m_scene = std::move(scene3d);
}

void SceneManager3D::render() {
    const auto& skybox = m_scene->m_skybox;

    if (skybox)
        skybox->cubemap->bind();

    auto& directionalLights = m_scene->m_ecsRegistry.getComponentView<components::DirectionalLightComponent>();
    auto& pointLights = m_scene->m_ecsRegistry.getComponentView<components::PointLightComponent>();
    auto& rendererComponents = m_scene->m_ecsRegistry.getComponentView<components::RendererComponent>();

    m_shadowSystem.beginDepthCapture();
    auto depthShader = m_shadowSystem.getDepthShader();
    for (auto& directionalLight : directionalLights) {
        m_shadowSystem.setShadowMap(directionalLight.shadowMap);

        for (auto& rendererComponent : rendererComponents) {
            depthShader->setUniform("lightSpaceMatrix", directionalLight.spaceMatrix);
            m_rendererSystem.render(rendererComponent, m_scene->m_camera, depthShader);
        }
    }
    m_shadowSystem.endDepthCapture();

    for (auto& rendererComponent : rendererComponents) {
        rendererComponent.shader->enable();

        m_lightSystem.prepareDirectionalLights(directionalLights, rendererComponent.shader);
        m_lightSystem.preparePointsLights(pointLights, rendererComponent.shader);

        m_rendererSystem.render(rendererComponent, m_scene->m_camera);

        rendererComponent.shader->disable();
    }

    m_pfxSystem.renderParticleEffects(
        m_scene->m_ecsRegistry.getComponentView<components::ParticleEffectComponent>(), m_scene->m_camera);

    if (skybox) {
        m_skyboxSystem.render(skybox->cubemap, skybox->shader, m_scene->m_camera);
        skybox->cubemap->unbind();
    }
}

void SceneManager3D::renderSceneGUI(gui::Window& window) {
    for (const auto& [name, id] : m_scene->m_ecsRegistry.getEntityNameToId()) {
        window.displayText(std::move(name));
        if (window.isPreviousWidgetClicked())
            m_activeEntity = m_scene->m_ecsRegistry.getEntityById(std::move(id));
    }
}

void SceneManager3D::renderMainGUI(gui::Window& window) {
    if (auto entity = m_activeEntity.lock()) {
        if (window.beginTreeNode("Selected entity")) {
            entity->onGUI(window);
            window.popTreeNode();
        }
    }
}
}
