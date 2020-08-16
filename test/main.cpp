#include "starlight/application/Entrypoint.hpp"
#include "starlight/application/context/ApplicationContext.h"
#include "starlight/core/path/PathManager.hpp"
#include "starlight/geometry/Geometry.hpp"
#include "starlight/geometry/Model.h"
#include "starlight/gui/GUIProxy.h"
#include "starlight/gui/Window.h"
#include "starlight/platform/shader/Shader.h"
#include "starlight/platform/texture/Cubemap.h"
#include "starlight/platform/texture/Texture.h"
#include "starlight/rendering/camera/EulerCamera.h"
#include "starlight/scene/Scene.h"
#include "starlight/scene/Skybox.h"
#include "starlight/scene/components/ModelComponent.h"
#include "starlight/scene/components/RendererComponent.h"

// pass somehow current time to renderers
// fix time
// rework profiler
// remove pointer references

using namespace sl;

class Context : public application::context::ApplicationContext {
public:
    void onInit() override {
        geometry::Geometry::init();
        // rework asset manager to be singleton/static class
        m_cubemap = asset::AssetManager::load<platform::texture::Cubemap>({ "/skybox/right.jpg",
            "/skybox/left.jpg",
            "/skybox/top.jpg",
            "/skybox/bottom.jpg",
            "/skybox/front.jpg",
            "/skybox/back.jpg" });

        m_cubemapShader = asset::AssetManager::load<platform::shader::Shader>({ "/cubemap.vert", "/cubemap.frag" });
        auto sshader = asset::AssetManager::load<platform::shader::Shader>(
            { "/el.vert", "/el.frag" });
        m_camera = rendering::camera::EulerCamera::create(glm::vec3(0.0f), 1.0f, 8.0f);

        m_shader = asset::AssetManager::load<platform::shader::Shader>({ "/t.vert", "/t.frag", "" });
        m_model = asset::AssetManager::load<geometry::Model>({ "/ground/untitled.obj" });

        m_skybox = std::make_shared<scene::Skybox>(scene::Skybox{ m_cubemapShader, m_cubemap });

        m_scene = std::make_shared<scene::Scene>();
        m_scene->setSkybox(m_skybox);

        m_sceneManager->setActiveScene(m_scene);

        auto entity = m_scene->addEntity();

        entity->addComponent<scene::components::ModelComponent>(m_model);
        entity->addComponent<scene::components::RendererComponent>(m_shader);

        // entity->addComponent<scene::components::ModelComponent>(m_model, m_shader);

        // auto groundEntity = std::make_shared<ecs::entity::Entity>("Ground");
        // auto modelComponent = std::make_shared<ecs::component::ModelComponent>(m_model, m_shader);

        // groundEntity->addComponent(modelComponent);
        // groundEntity->addComponent(std::make_shared<ecs::component::TransformComponent>());

        // auto towerEntity = std::make_shared<ecs::entity::Entity>("ExampleEntity");
        // auto towerModel = m_applicationContextResource.assetManager.load<geometry::Model>({ "/tow/tower.obj" });
        // auto towerModelComponent = std::make_shared<ecs::component::ModelComponent>(towerModel, m_shader);

        // towerEntity->addComponent(towerModelComponent);
        // towerEntity->addComponent(std::make_shared<ecs::component::TransformComponent>());

        // auto meshGridEntity = std::make_shared<ecs::entity::MeshGridEntity>("Water", sshader);
        // meshGridEntity->addComponent(std::make_shared<ecs::component::TransformComponent>());

        // m_scene->addEntity(groundEntity);
        // m_scene->addEntity(towerEntity);
        // m_scene->addEntity(meshGridEntity);
    }

    void onAttach() override {
    }

    void onDetach() override {
    }

    void renderGUI(std::shared_ptr<gui::GUIProxy>& guiProxy) override {
        static float width = 300;
        auto vp = m_viewport;

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

    void handleInput(std::unique_ptr<platform::input::Input>& input) override {
        m_camera->handleInput(input);
    }

    void update(float deltaTime) override {
        m_camera->update(deltaTime);
        m_sceneManager->update(deltaTime);
    }

    void render() override {
        m_sceneManager->render(m_camera);
    }

private:
    std::shared_ptr<platform::shader::Shader> m_cubemapShader;
    std::shared_ptr<platform::shader::Shader> m_shader;
    std::shared_ptr<platform::texture::Cubemap> m_cubemap;
    std::shared_ptr<rendering::camera::UserControllableCamera> m_camera;
    std::shared_ptr<geometry::Model> m_model;
    std::shared_ptr<rendering::data::ModelData> m_modelRenderData;
    std::shared_ptr<scene::Scene> m_scene;
    std::shared_ptr<scene::Skybox> m_skybox;
};

class App : public sl::application::Application {
    using sl::application::Application::Application;

public:
    void preInit() override {
        initPaths();
    }

    virtual void onStart() override {
        context = createContext<Context>();
        switchContext(context);
    }

    void initPaths() {
        sl::core::path::PathManager::registerResourcePath<sl::platform::shader::Shader>(SHADERS_DIR);
        sl::core::path::PathManager::registerResourcePath<sl::platform::texture::Texture>(TEXTURES_DIR);
        sl::core::path::PathManager::registerResourcePath<sl::platform::texture::Cubemap>(CUBEMAPS_DIR);
        sl::core::path::PathManager::registerResourcePath<sl::geometry::Model>(MODELS_DIR);
    }

private:
    std::shared_ptr<Context> context;
};

STARLIGHT_ENTRYPOINT(App);
