#pragma once

#include "DebugConsole.hpp"
#include "gui/EditorGui.h"
#include "sl/app/ApplicationContext.h"
#include "sl/app/Deserializer.h"
#include "sl/app/Serializer.h"
#include "sl/asset/AssetManager.h"
#include "sl/core/BaseError.hpp"
#include "sl/core/FileSystem.h"
#include "sl/core/Input.h"
#include "sl/core/Logger.h"
#include "sl/ecs/Entity.h"
#include "sl/event/Categories.h"
#include "sl/event/Event.h"
#include "sl/gfx/ViewFrustum.h"
#include "sl/gfx/camera/EulerCamera.h"
#include "sl/gfx/camera/FPSCamera.h"
#include "sl/gui/ErrorDialog.hpp"
#include "sl/gui/GuiApi.h"
#include "sl/gui/Utils.hpp"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/scene/Scene.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"

#include <memory>

using namespace sl;
using namespace sl::scene;
using namespace sl::core;

class EditorContext : public app::ApplicationContext {
public:
    explicit EditorContext(const std::string& ident)
        : ApplicationContext(ident) {
    }

    void onInit() override {
        auto [windowWidth, windowHeight] = m_windowProxy->getSize();

        auto viewFrustum = gfx::ViewFrustum { windowWidth, windowHeight };
        m_activeCamera = std::make_shared<gfx::camera::EulerCamera>(viewFrustum, math::Vec3(0.0f), 1.0f, 8.0f);
        m_scene = scene::Scene::create();

        // TODO: remove!
        auto& viewport = m_activeCamera->viewFrustum.viewport;
        auto guiSharedState = std::make_shared<editor::gui::SharedState>(m_assetManager, viewport.width, viewport.height);
        m_editorGui = std::make_shared<editor::gui::EditorGui>(guiSharedState);

        m_scene->camera = m_activeCamera;
        m_editorGui->sharedState->activeScene = m_scene;

        recalculateWindow(windowWidth, windowHeight);

        WRITE_DEBUG("{}", "Editor context initialized");
    }

    void onAttach() override {
    }

    void onDetach() override {
    }

    void renderGui(gui::GuiApi& gui) override {
        m_editorGui->renderEditorGui(gui);
        m_errorDialog.show(gui);
    }

    void update(scene::SceneSystems& sceneSystems, float deltaTime, float time, core::Input& input) override {
        m_activeCamera->update(deltaTime, input);
        auto pfxs = m_scene->ecsRegistry.getComponentView<components::ParticleEffectComponent>();
        sceneSystems.pfxSystem.update(pfxs, deltaTime, m_scene->camera);
    }

    void render(gfx::Renderer& renderer) override {
        const auto& skybox = m_scene->skybox;

        if (skybox)
            skybox->cubemap->bind();

        auto directionalLights = m_scene->ecsRegistry.getComponentView<components::DirectionalLightComponent>();
        auto pointLights = m_scene->ecsRegistry.getComponentView<components::PointLightComponent>();
        auto rendererComponents = m_scene->ecsRegistry.getComponentView<components::MeshRendererComponent>();
        auto transforms = m_scene->ecsRegistry.getComponentView<components::TransformComponent>();
        auto models = m_scene->ecsRegistry.getComponentView<components::ModelComponent>();
        auto materials = m_scene->ecsRegistry.getComponentView<components::MaterialComponent>();

        renderer.beginDepthCapture();
        auto depthShader = renderer.getDepthShader();

        for (auto& directionalLight : directionalLights) {
            renderer.setShadowMap(directionalLight.shadowMap);

            for (auto& rendererComponent : rendererComponents) {
                depthShader->setUniform("lightSpaceMatrix", directionalLight.spaceMatrix);
                renderer.renderModel(rendererComponent, materials, models, transforms, *m_scene->camera, *depthShader);
            }
        }

        renderer.endDepthCapture();

        for (auto& rendererComponent : rendererComponents) {
            auto& shader = rendererComponent.shader;

            shader->enable();

            renderer.prepareDirectionalLights(directionalLights, shader);
            renderer.preparePointsLights(pointLights, transforms, shader);
            renderer.renderModel(rendererComponent, materials, models, transforms, *m_scene->camera);

            shader->disable();
        }

        renderer.renderParticleEffects(
            m_scene->ecsRegistry.getComponentView<components::ParticleEffectComponent>(), transforms, m_scene->camera);

        if (skybox) {
            renderer.renderCubemap(skybox->cubemap, skybox->shader, m_scene->camera);
            skybox->cubemap->unbind();
        }
    }

    void handleEvents(const xvent::EventProvider& eventProvider) override {
        auto events = eventProvider.getByCategories<event::CoreCategory, event::EditorCategory>();

        using namespace sl::event;

        for (auto& event : events) {
            if (event->is<SetSkyboxEvent>()) {
                auto cubemap = event->as<SetSkyboxEvent>()->cubemap;
                auto skybox = sl::scene::Skybox::create(GLOBALS().shaders->defaultCubemapShader, cubemap);
                m_scene->skybox = skybox;
                break;
            }

            if (event->is<QuitEvent>())
                m_windowProxy->quit();

            if (event->is<WindowResizedEvent>()) {
                auto windowResizedEvent = event->as<WindowResizedEvent>();
                auto [width, height] = windowResizedEvent->getSize();

                recalculateWindow(width, height);
                break;
            }

            if (event->is<ChangeSceneCenterEvent>()) {
                auto& newCenter = event->as<ChangeSceneCenterEvent>()->center;
                auto sceneCamera = std::dynamic_pointer_cast<sl::gfx::camera::EulerCamera>(m_activeCamera);
                if (sceneCamera)
                    sceneCamera->setCenter(newCenter);
            }

            try {
                using namespace sl::app;

                if (event->is<SerializeSceneEvent>())
                    Serializer { ".", "scene" }.serialize(m_assetManager, m_scene);

                if (event->is<DeserializeSceneEvent>())
                    Deserializer { m_assetManager, m_scene }.deserialize("./scene.starscene");

            } catch (sl::core::Error& err) {
                m_errorDialog.setErrorMessage(err.as<std::string>());
            }
        }
    }

    void recalculateWindow(float width, float height) {
        editor::gui::GuiProperties guiProperties {
            static_cast<int>(width), static_cast<int>(height)
        };
        m_editorGui->sharedState->guiProperties = guiProperties;

        gfx::ViewFrustum::Viewport newViewport {
            static_cast<int>(width - guiProperties.scenePanelProperties.size.x - guiProperties.rightPanelProperties.size.x),
            static_cast<int>(height - guiProperties.bottomPanelProperties.size.y),
            static_cast<int>(guiProperties.scenePanelProperties.size.x),
            static_cast<int>(guiProperties.bottomPanelProperties.size.y)
        };

        m_activeCamera->viewFrustum.viewport = newViewport;
        m_activeCamera->calculateProjectionMatrix();

        m_lowLevelRendererProxy->setViewport(newViewport);
    }

private:
    sl::asset::AssetManager m_assetManager;

    std::shared_ptr<editor::gui::EditorGui> m_editorGui;
    std::shared_ptr<gfx::camera::Camera> m_activeCamera;
    std::shared_ptr<scene::Scene> m_scene;

    sl::gui::ErrorDialog m_errorDialog;
};
