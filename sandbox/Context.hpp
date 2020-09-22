#pragma once

#include "sl/application/Entrypoint.hpp"
#include "sl/application/context/ApplicationContext.h"
#include "sl/asset/AssetManager.hpp"
#include "sl/core/log/Logger.h"
#include "sl/core/path/PathManager.hpp"
#include "sl/geometry/Geometry.hpp"
#include "sl/geometry/Model.h"
#include "sl/gui/GUIProxy.h"
#include "sl/gui/Window.h"
#include "sl/misc/colors.hpp"
#include "sl/platform/shader/Shader.h"
#include "sl/platform/texture/Cubemap.h"
#include "sl/platform/texture/Texture.h"
#include "sl/platform/time/Clock.h"
#include "sl/rendering/camera/EulerCamera.h"
#include "sl/scene/Scene.h"
#include "sl/scene/Skybox.h"

#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/PFXComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/RendererComponent.h"
#include "sl/scene/components/TransformComponent.h"

using namespace sl;
using namespace sl::scene;

class Context : public application::context::ApplicationContext {
public:
    void onInit() override {
        // rework asset manager to be singleton/static class
        m_cubemap = asset::AssetManager::load<platform::texture::Cubemap>("/skybox/right.jpg",
            "/skybox/left.jpg",
            "/skybox/top.jpg",
            "/skybox/bottom.jpg",
            "/skybox/front.jpg",
            "/skybox/back.jpg");

        m_cubemapShader = asset::AssetManager::load<platform::shader::Shader>("/cubemap.vert", "/cubemap.frag");
        auto sshader = asset::AssetManager::load<platform::shader::Shader>("/el.vert", "/el.frag");
        m_camera = rendering::camera::EulerCamera::create(math::Vec3(0.0f), 1.0f, 8.0f);

        m_shader = asset::AssetManager::load<platform::shader::Shader>("/t.vert", "/t.frag", "");
        m_model = asset::AssetManager::load<geometry::Model>("/ground/untitled.obj");

        m_skybox = scene::Skybox::create(m_cubemapShader, m_cubemap);
        m_scene = scene::Scene::create();

        m_scene->setSkybox(m_skybox);
        m_sceneManager->setActiveScene(m_scene);

        auto entity = m_scene->addEntity("Ground");

        entity->addComponent<components::ModelComponent>(m_model);
        entity->addComponent<components::RendererComponent>(m_shader);
        entity->addComponent<components::TransformComponent>();
        entity->addComponent<components::MaterialComponent>();

        // auto towerModel = asset::AssetManager::load<geometry::Model>("/tow/tower.obj");
        // auto entity2 = m_scene->addEntity("Tower");

        // entity2->addComponent<components::ModelComponent>(towerModel);
        // entity2->addComponent<components::RendererComponent>(m_shader);
        // entity2->addComponent<components::TransformComponent>();

        auto sun = m_scene->addEntity("Sun");
        sun->addComponent<components::DirectionalLightComponent>(math::Vec3{ 1.0f, 1.0f, 1.0f });

        // m_lightSource = m_scene->addEntity("LightSource");
        // m_lightSource->addComponent<components::TransformComponent>();
        // m_lightSource->addComponent<components::ModelComponent>(geometry::Geometry::getSquare());
        // m_lightSource->addComponent<components::RendererComponent>(sshader);
        // m_lightSource->addComponent<components::PointLightComponent>();

        auto pfxSource = m_scene->addEntity("PFX");
        pfxSource->addComponent<components::TransformComponent>();
        // pfxSource->addComponent<components::ModelComponent>(geometry::Geometry::getSquare());
        // TODO: renderer component need model, we should check if model compnent exist
        // pfxSource->addComponent<components::RendererComponent>(sshader);
        pfxSource->addComponent<components::PFXComponent>();
    }

    void onAttach() override {
    }

    void onDetach() override {
    }

    void renderGUI(std::shared_ptr<gui::GUIProxy> guiProxy) override {
        static float width = 300;
        auto vp = m_viewport;

        // TODO: rework api, hierarchy is too deep and this shouldn't be based on RAII
        {
            auto rightWindow = guiProxy->createWindow("Starlight", { vp.width - width, 250 }, { width, 500 });
            m_camera->onGUI(rightWindow);
            m_sceneManager->renderMainGUI(rightWindow);
            if (rightWindow.beginTreeNode("Assets")) {
                rightWindow.popTreeNode();
            }
        }
        {
            auto leftWindow = guiProxy->createWindow("Scene", { 0, 250 }, { width, 350 });
            m_sceneManager->renderSceneGUI(leftWindow);
        }
    }

    void handleInput(std::shared_ptr<platform::input::Input> input) override {
        m_camera->handleInput(input);
    }

    void update(float deltaTime, float time) override {
        m_camera->update(deltaTime);
        m_sceneManager->update(deltaTime);

        // static float f = 1.0f;
        // static float R = 5.0f;

        // auto& transform = m_lightSource->getComponent<components::TransformComponent>();
        // transform.position.x = R * std::cos(f * time);
        // transform.position.z = R * std::sin(f * time);
    }

    void render() override {
        m_sceneManager->render(m_camera);
    }

private:
    std::shared_ptr<ecs::Entity> m_lightSource;
    std::shared_ptr<platform::shader::Shader> m_cubemapShader;
    std::shared_ptr<platform::shader::Shader> m_shader;
    std::shared_ptr<platform::texture::Cubemap> m_cubemap;
    std::shared_ptr<rendering::camera::UserControllableCamera> m_camera;
    std::shared_ptr<geometry::Model> m_model;
    std::shared_ptr<rendering::data::ModelData> m_modelRenderData;
    std::shared_ptr<scene::Scene> m_scene;
    std::shared_ptr<scene::Skybox> m_skybox;
};
