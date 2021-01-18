#pragma once

#include "editor/gui/Settings.h"
#include "gui/EditorGui.h"
#include "res/ResourceManager.h"
#include "sl/application/ApplicationContext.h"
#include "sl/asset/AssetManager.hpp"
#include "sl/core/FileSystem.h"
#include "sl/core/Logger.h"
#include "sl/ecs/Entity.h"
#include "sl/event/Categories.h"
#include "sl/event/Event.h"
#include "sl/graphics/camera/EulerCamera.h"
#include "sl/graphics/camera/FPSCamera.h"
#include "sl/gui/GuiApi.h"
#include "sl/gui/Utils.hpp"
#include "sl/scene/Scene.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"
#include "DebugConsole.hpp"


using namespace sl;
using namespace sl::scene;
using namespace sl::core;

class EditorContext : public application::ApplicationContext {
    SL_CONTEXT;

public:
    void onInit() override {
        m_activeCamera = graphics::camera::EulerCamera::create(math::Vec3(0.0f), 1.0f, 8.0f);
        m_scene = scene::Scene::create();
        m_editorGui = std::make_shared<editor::gui::EditorGui>(createGuiSettings(), m_entities, m_resourceManager);

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
    }

    void handleInput(std::shared_ptr<core::Input> input) override {
        m_activeCamera->handleInput(input);
    }

    void update(scene::SceneSystems& sceneSystems, float deltaTime, float time) override {
        m_activeCamera->update(deltaTime);
        auto pfxs = m_scene->ecsRegistry.getComponentView<components::ParticleEffectComponent>();
        sceneSystems.pfxSystem.update(pfxs, deltaTime, m_scene->camera);
    }

    void render(scene::SceneSystems& sceneSystems) override {
        const auto& skybox = m_scene->skybox;

        if (skybox)
            skybox->cubemap->bind();

        auto directionalLights = m_scene->ecsRegistry.getComponentView<components::DirectionalLightComponent>();
        auto pointLights = m_scene->ecsRegistry.getComponentView<components::PointLightComponent>();
        auto rendererComponents = m_scene->ecsRegistry.getComponentView<components::RendererComponent>();
        auto transforms = m_scene->ecsRegistry.getComponentView<components::TransformComponent>();
	    auto models = m_scene->ecsRegistry.getComponentView<components::ModelComponent>();
        auto materials = m_scene->ecsRegistry.getComponentView<components::MaterialComponent>();

        sceneSystems.shadowSystem.beginDepthCapture();
        auto depthShader = sceneSystems.shadowSystem.getDepthShader();
        for (auto& directionalLight : directionalLights) {
            sceneSystems.shadowSystem.setShadowMap(directionalLight.shadowMap);

            for (auto& rendererComponent : rendererComponents) {
                depthShader->setUniform("lightSpaceMatrix", directionalLight.spaceMatrix);
                sceneSystems.rendererSystem.render(rendererComponent, materials, models, transforms, m_scene->camera, depthShader);
            }
        }
        sceneSystems.shadowSystem.endDepthCapture();

 
        for (auto& rendererComponent : rendererComponents) {
            rendererComponent.shader->enable();

            sceneSystems.lightSystem.prepareDirectionalLights(directionalLights, rendererComponent.shader);
            sceneSystems.lightSystem.preparePointsLights(pointLights, transforms, rendererComponent.shader);

            sceneSystems.rendererSystem.render(rendererComponent, materials, models, transforms, m_scene->camera);

            rendererComponent.shader->disable();
        }

        sceneSystems.pfxSystem.renderParticleEffects(
            m_scene->ecsRegistry.getComponentView<components::ParticleEffectComponent>(), transforms, m_scene->camera);

        if (skybox) {
            sceneSystems.skyboxSystem.render(skybox->cubemap, skybox->shader, m_scene->camera);
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
        }
    }

private:
    void loadDefaultShaders() {
        auto shaderResource = std::make_shared<editor::res::ShaderResource>(sl::utils::Globals::shaders->defaultModelShader,
            "defaultShader");
        m_resourceManager.addResource(shaderResource);
    }

    editor::res::ResourceManager m_resourceManager;

    std::shared_ptr<editor::gui::EditorGui> m_editorGui;
    std::vector<std::shared_ptr<ecs::Entity>> m_entities;

    std::shared_ptr<graphics::camera::UserControllableCamera> m_activeCamera;
    std::shared_ptr<scene::Scene> m_scene;
};
