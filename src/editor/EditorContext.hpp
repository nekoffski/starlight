#pragma once

#include "DebugConsole.hpp"
#include "editor/gui/Settings.h"
#include "gui/EditorGui.h"
#include "sl/application/ApplicationContext.h"
#include "sl/application/Deserializer.h"
#include "sl/application/Serializer.h"
#include "sl/asset/AssetLoader.hpp"
#include "sl/asset/AssetManager.h"
#include "sl/core/FileSystem.h"
#include "sl/core/Logger.h"
#include "sl/core/error/BaseError.hpp"
#include "sl/ecs/Entity.h"
#include "sl/event/Categories.h"
#include "sl/event/Event.h"
#include "sl/graphics/camera/EulerCamera.h"
#include "sl/graphics/camera/FPSCamera.h"
#include "sl/gui/ErrorDialog.hpp"
#include "sl/gui/GuiApi.h"
#include "sl/gui/Utils.hpp"
#include "sl/scene/Scene.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"

using namespace sl;
using namespace sl::scene;
using namespace sl::core;

class EditorContext : public application::ApplicationContext {
    SL_CONTEXT;

public:
    void onInit() override {
        m_activeCamera = graphics::camera::EulerCamera::create(math::Vec3(0.0f), 1.0f, 8.0f);
        m_scene = scene::Scene::create();
        m_editorGui = std::make_shared<editor::gui::EditorGui>(createGuiSettings(), m_entities, m_assetManager);

        m_scene->camera = m_activeCamera;
        m_editorGui->setActiveScene(m_scene);

        loadDefaultShaders();

        WRITE_DEBUG("%s", "Editor context initialized");
    }

    void onAttach() override {
    }

    void onDetach() override {
    }

    editor::gui::Settings createGuiSettings() {
        // const auto viewport = m_window->getParams().viewport;
        const float leftPanelWidth = 0.15f;
        const float leftPanelTopBottomRatio = 0.5f;

        return editor::gui::Settings{ 1600, 900, leftPanelWidth, leftPanelTopBottomRatio };
    }

    void renderGui(gui::GuiApi& gui) override {
        m_editorGui->renderEditorGui(gui);
        m_errorDialog.show(gui);
    }

    void handleInput(std::shared_ptr<core::Input> input) override {
        m_activeCamera->handleInput(input);
    }

    void update(std::shared_ptr<scene::SceneSystems> sceneSystems, float deltaTime, float time) override {
        m_activeCamera->update(deltaTime);
        auto pfxs = m_scene->ecsRegistry.getComponentView<components::ParticleEffectComponent>();
        sceneSystems->pfxSystem.update(pfxs, deltaTime, m_scene->camera);
    }

    void render(std::shared_ptr<graphics::Renderer> renderer) override {
        const auto& skybox = m_scene->skybox;

        if (skybox)
            skybox->cubemap->bind();

        auto directionalLights = m_scene->ecsRegistry.getComponentView<components::DirectionalLightComponent>();
        auto pointLights = m_scene->ecsRegistry.getComponentView<components::PointLightComponent>();
        auto rendererComponents = m_scene->ecsRegistry.getComponentView<components::RendererComponent>();
        auto transforms = m_scene->ecsRegistry.getComponentView<components::TransformComponent>();
        auto models = m_scene->ecsRegistry.getComponentView<components::ModelComponent>();
        auto materials = m_scene->ecsRegistry.getComponentView<components::MaterialComponent>();

        renderer->beginDepthCapture();
        auto depthShader = renderer->getDepthShader();

        for (auto& directionalLight : directionalLights) {
            renderer->setShadowMap(directionalLight.shadowMap);

            for (auto& rendererComponent : rendererComponents) {
                depthShader->setUniform("lightSpaceMatrix", directionalLight.spaceMatrix);
                renderer->renderModel(rendererComponent, materials, models, transforms, m_scene->camera, depthShader);
            }
        }
        renderer->endDepthCapture();

        for (auto& rendererComponent : rendererComponents) {
            rendererComponent.shader->enable();

            renderer->prepareDirectionalLights(directionalLights, rendererComponent.shader);
            renderer->preparePointsLights(pointLights, transforms, rendererComponent.shader);

            renderer->renderModel(rendererComponent, materials, models, transforms, m_scene->camera);

            rendererComponent.shader->disable();
        }

        renderer->renderParticleEffects(
            m_scene->ecsRegistry.getComponentView<components::ParticleEffectComponent>(), transforms, m_scene->camera);

        if (skybox) {
            renderer->renderCubemap(skybox->cubemap, skybox->shader, m_scene->camera);
            skybox->cubemap->unbind();
        }
    }

    void handleEvents(const xvent::EventProvider& eventProvider) override {
        auto events = eventProvider.getByCategories<event::CoreCategory, event::EditorCategory>();

        for (auto event : events) {
            if (event->is<event::AddEntityEvent>()) {
                auto entityName = event->as<event::AddEntityEvent>()->name;
                m_entities.emplace_back(m_scene->addEntity(std::move(entityName)));
                break;
            }

            if (event->is<event::SetSkyboxEvent>()) {
                auto cubemap = event->as<event::SetSkyboxEvent>()->cubemap;
                auto skybox = sl::scene::Skybox::create(sl::utils::Globals::shaders->defaultCubemapShader, cubemap);
                m_scene->skybox = skybox;
                break;
            }

            if (event->is<event::WindowResizedEvent>()) {
                const float leftPanelWidth = 0.2f;
                const float leftPanelTopBottomRatio = 0.5f;

                auto windowResizedEvent = event->as<event::WindowResizedEvent>();

                editor::gui::Settings settings{ windowResizedEvent->width, windowResizedEvent->height, leftPanelWidth, leftPanelTopBottomRatio };
                m_editorGui->setSettings(settings);
                break;
            }

            try {
                if (event->is<event::SerializeSceneEvent>()) {
                    sl::application::Serializer serializer{ ".", "scene" };
                    serializer.serialize(m_assetManager, m_scene);
                }

                if (event->is<event::DeserializeSceneEvent>()) {
                    sl::application::Deserializer deserializer{ m_assetManager, m_scene };
                    deserializer.deserialize("./scene.starscene");
                }
            } catch (sl::core::error::Error& err) {
                m_errorDialog.setErrorMessage(err.as<std::string>());
            }
        }
    }

private:
    void loadDefaultShaders() {
        auto shaderAsset = std::make_shared<sl::asset::ShaderAsset>(sl::utils::Globals::shaders->defaultModelShader,
            "defaultShader");
        shaderAsset->shouldSerialize = false;
        m_assetManager.addAsset(shaderAsset);
    }

    sl::asset::AssetManager m_assetManager;

    std::shared_ptr<editor::gui::EditorGui> m_editorGui;
    std::vector<std::shared_ptr<ecs::Entity>> m_entities;

    std::shared_ptr<graphics::camera::UserControllableCamera> m_activeCamera;
    std::shared_ptr<scene::Scene> m_scene;

    sl::gui::ErrorDialog m_errorDialog;
};
