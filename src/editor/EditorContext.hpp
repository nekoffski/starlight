#pragma once

#include "editor/gui/Settings.h"
#include "gui/EditorGui.h"
#include "res/ResourceManager.h"
#include "sl/application/Entrypoint.hpp"
#include "sl/application/context/ApplicationContext.h"
#include "sl/core/event/Event.h"
#include "sl/core/event/EventBus.h"
#include "sl/core/event/EventObserver.h"
#include "sl/core/event/EventPool.h"
#include "sl/core/fs/FileSystem.h"
#include "sl/core/log/Logger.h"
#include "sl/ecs/Entity.h"
#include "sl/gui/GuiProxy.h"
#include "sl/gui/Utils.hpp"
#include "sl/rendering/camera/EulerCamera.h"
#include "sl/rendering/camera/FPSCamera.h"
#include "sl/scene/Scene3D.h"

#include <filesystem>

using namespace sl;
using namespace sl::scene;
using namespace sl::core;

class EditorContext : public event::EventObserver, public application::context::ApplicationContext {
public:
    void onInit() override {
        m_activeCamera = rendering::camera::EulerCamera::create(math::Vec3(0.0f), 1.0f, 8.0f);
        m_scene = scene::Scene3D::create();
        m_editorGui = std::make_shared<editor::gui::EditorGui>(createGuiSettings(), m_entities, m_resourceManager);

        m_scene->setCamera(m_activeCamera);
        m_sceneManager->setActiveScene(m_scene);

        event::EventBus::registerEventObserver(this);
    }

    void onAttach() override {
    }

    void onDetach() override {
    }

    editor::gui::Settings createGuiSettings() {
        const auto viewport = m_window->getParams().viewport;
        const float leftPanelWidth = 0.2f;
        const float leftPanelTopBottomRatio = 0.5f;

        return editor::gui::Settings{ viewport.width, viewport.height, leftPanelWidth, leftPanelTopBottomRatio };
    }

    void renderGui(gui::GuiProxy& gui) override {
        m_editorGui->renderEditorGui(gui);
    }

    void handleInput(std::shared_ptr<platform::input::Input> input) override {
        m_activeCamera->handleInput(input);
    }

    void update(float deltaTime, float time) override {
        m_activeCamera->update(deltaTime);
        m_sceneManager->update(deltaTime);
    }

    void render() override {
        m_sceneManager->render();
    }

    void handleEvents(event::EventPool& eventPool) override {
        auto& editorEvents = eventPool.getEventsByCategory(event::EventCategory::EDITOR);

        for (auto& event : editorEvents) {
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
                m_scene->setSkybox(skybox);

                break;
            }
            }
        }
    }

private:
    editor::res::ResourceManager m_resourceManager;

    std::shared_ptr<editor::gui::EditorGui> m_editorGui;
    std::vector<std::shared_ptr<ecs::Entity>> m_entities;

    std::shared_ptr<rendering::camera::UserControllableCamera> m_activeCamera;
    std::shared_ptr<scene::Scene3D> m_scene;
};
