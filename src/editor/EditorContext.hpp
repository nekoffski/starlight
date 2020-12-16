#pragma once

#include "editor/gui/Settings.h"
#include "gui/EditorGui.h"
#include "res/ResourceManager.h"
#include "sl/application/ApplicationContext.h"
#include "sl/asset/AssetManager.hpp"
#include "sl/core/FileSystem.h"
#include "sl/core/Logger.h"
#include "sl/ecs/Entity.h"
#include "sl/event/Event.h"
#include "sl/event/EventBus.h"
#include "sl/event/EventObserver.h"
#include "sl/event/EventPool.h"
#include "sl/gui/GuiApi.h"
#include "sl/gui/Utils.hpp"
#include "sl/graphics/camera/EulerCamera.h"
#include "sl/graphics/camera/FPSCamera.h"
#include "sl/scene/Scene.h"

#include <filesystem>

using namespace sl;
using namespace sl::scene;
using namespace sl::core;

class EditorContext : public event::EventObserver, public application::ApplicationContext {
public:
    void onInit() override {
        m_activeCamera = graphics::camera::EulerCamera::create(math::Vec3(0.0f), 1.0f, 8.0f);
        m_scene = scene::Scene::create();
        m_editorGui = std::make_shared<editor::gui::EditorGui>(createGuiSettings(), m_entities, m_resourceManager);

        m_scene->camera = m_activeCamera;

        event::EventBus::registerEventObserver(this);
        loadBaseShaders();
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

    void handleInput(std::shared_ptr<platform::input::Input> input) override {
        m_activeCamera->handleInput(input);
    }

    void update(float deltaTime, float time) override {
        m_activeCamera->update(deltaTime);
        //auto& pfxs = m_scene->m_ecsRegistry.getComponentView<components::ParticleEffectComponent>();
        //pfxSystem.update(pfxs, deltaTime, m_scene->m_camera);
    }

    void render(scene::SceneSystems& sceneSystems) override {
        const auto& skybox = m_scene->skybox;

        if (skybox)
            skybox->cubemap->bind();

        auto& directionalLights = m_scene->ecsRegistry.getComponentView<components::DirectionalLightComponent>();
        auto& pointLights = m_scene->ecsRegistry.getComponentView<components::PointLightComponent>();
        auto& rendererComponents = m_scene->ecsRegistry.getComponentView<components::RendererComponent>();

        sceneSystems.shadowSystem.beginDepthCapture();
        auto depthShader = sceneSystems.shadowSystem.getDepthShader();
        for (auto& directionalLight : directionalLights) {
            sceneSystems.shadowSystem.setShadowMap(directionalLight.shadowMap);

            for (auto& rendererComponent : rendererComponents) {
                depthShader->setUniform("lightSpaceMatrix", directionalLight.spaceMatrix);
                sceneSystems.rendererSystem.render(rendererComponent, m_scene->camera, depthShader);
            }
        }
        sceneSystems.shadowSystem.endDepthCapture();

        for (auto& rendererComponent : rendererComponents) {
            rendererComponent.shader->enable();

            sceneSystems.lightSystem.prepareDirectionalLights(directionalLights, rendererComponent.shader);
            sceneSystems.lightSystem.preparePointsLights(pointLights, rendererComponent.shader);

            sceneSystems.rendererSystem.render(rendererComponent, m_scene->camera);

            rendererComponent.shader->disable();
        }

        sceneSystems.pfxSystem.renderParticleEffects(
            m_scene->ecsRegistry.getComponentView<components::ParticleEffectComponent>(), m_scene->camera);

        if (skybox) {
            sceneSystems.skyboxSystem.render(skybox->cubemap, skybox->shader, m_scene->camera);
            skybox->cubemap->unbind();
        }
    }

    void handleEvents(event::EventPool& eventPool) override {
        auto events = eventPool.getEventsByCategory({ event::EventCategory::EDITOR, event::EventCategory::CORE });

        for (auto& event : events) {
            switch (event->getType()) {
            case event::EventType::ADD_ENTITY: {
                auto entityName = event->as<event::AddEntityEvent>()->name;
                m_entities.emplace_back(m_scene->addEntity(std::move(entityName)));
                break;
            }

            case event::EventType::SET_SKYBOX: {
                auto cubemap = event->as<event::SetSkyboxEvent>()->cubemap;
                auto cubemapShader =
                    asset::AssetManager::loadLocalPath<platform::shader::Shader>("/cubemap.vert", "/cubemap.frag");
                auto skybox = sl::scene::Skybox::create(cubemapShader, cubemap);
                m_scene->skybox = skybox;
                break;
            }

            case event::EventType::WINDOW_RESIZED: {
                const float leftPanelWidth = 0.2f;
                const float leftPanelTopBottomRatio = 0.5f;

                auto windowResizedEvent = event->as<event::WindowResizedEvent>();

                editor::gui::Settings settings{ windowResizedEvent->width, windowResizedEvent->height, leftPanelWidth, leftPanelTopBottomRatio };
                m_editorGui->setSettings(settings);
                break;
            }
            }
        }
    }

private:
    void loadBaseShaders() {
        const std::vector<std::string> shadersToLoad = {
            "/t"
        };

        for (auto& shaderToLoad : shadersToLoad) {
            auto shader = sl::asset::AssetManager::loadLocalPath<sl::platform::shader::Shader>(
                shaderToLoad + ".vert", shaderToLoad + ".frag");
            auto shaderResource = std::make_shared<editor::res::ShaderResource>(shader, shaderToLoad);
            m_resourceManager.addResource(shaderResource);
        }
    }

    editor::res::ResourceManager m_resourceManager;

    std::shared_ptr<editor::gui::EditorGui> m_editorGui;
    std::vector<std::shared_ptr<ecs::Entity>> m_entities;

    std::shared_ptr<graphics::camera::UserControllableCamera> m_activeCamera;
    std::shared_ptr<scene::Scene> m_scene;
};
