#include <starlight/application/Entrypoint.hpp>
#include <starlight/application/context/ApplicationContext.h>
#include <starlight/core/path/PathManager.hpp>
#include <starlight/framework/graphics/camera/EulerCamera.h>
#include <starlight/geometry/Model.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/platform/texture/Cubemap.h>
#include <starlight/platform/texture/Texture.h>
#include <starlight/scene/Scene.h>
#include <starlight/scene/Skybox.h>

using namespace starl;

class Context : public application::context::ApplicationContext {
public:
    Context(application::context::ApplicationContextResource&& acr)
        : ApplicationContext(std::move(acr)) {
        m_cubemap = m_applicationContextResource.assetManager.load<texture::Cubemap>({ "/skybox/posx.jpg",
            "/skybox/negx.jpg",
            "/skybox/posy.jpg",
            "/skybox/negy.jpg",
            "/skybox/posz.jpg",
            "/skybox/negz.jpg" });
        m_cubemapShader = m_applicationContextResource.assetManager.load<shader::Shader>({ "/cubemap.vert", "/cubemap.frag" });
        m_camera = framework::graphics::camera::EulerCamera::create(glm::vec3(0.0f), 1.0f, 8.0f);

        m_applicationContextResource.cubemapRenderer->setCamera(m_camera);
        m_applicationContextResource.modelRenderer->setCamera(m_camera);

        m_shader = m_applicationContextResource.assetManager.load<shader::Shader>({ "/t.vert", "/t.frag", "" });
        m_model = m_applicationContextResource.assetManager.load<geometry::Model>({ "/tow/tower.obj" });
        m_modelRenderData = std::make_shared<rendering::data::ModelRenderData>(m_model);
        m_skybox = std::make_shared<scene::Skybox>(scene::Skybox{ m_cubemapShader, m_cubemap });

        m_scene = std::make_shared<scene::Scene>();
        m_scene->setCamera(m_camera);
        m_scene->setSkybox(m_skybox);

        models.insert({ m_shader, { m_modelRenderData } });

        auto entity = std::make_shared<ecs::entity::Entity>();
        auto modelComponent = std::make_shared<ecs::component::ModelComponent>(m_modelRenderData, m_shader);
        entity->addComponent(modelComponent);

        m_scene->addEntity(entity);

        m_applicationContextResource.sceneManager->setActiveScene(m_scene);
    }

    void onAttach() override {
    }

    void onDetach() override {
    }

    void handleInput(std::unique_ptr<platform::input::Input>& input) override {
        m_camera->handleInput(input);
    }

    void update(float deltaTime) override {
        m_camera->update(deltaTime);
        m_applicationContextResource.sceneManager->update(deltaTime);
    }

    void render() override {
        // m_applicationContextResource.cubemapRenderer->render();
        // TODO: pass camera as const ref instead of setting it once
        m_applicationContextResource.sceneManager->render();
    }

private:
    std::shared_ptr<platform::shader::Shader> m_cubemapShader;
    std::shared_ptr<platform::shader::Shader> m_shader;
    std::shared_ptr<platform::texture::Cubemap> m_cubemap;
    std::shared_ptr<framework::graphics::camera::UserControllableCamera> m_camera;
    std::shared_ptr<geometry::Model> m_model;
    std::shared_ptr<rendering::data::ModelRenderData> m_modelRenderData;
    std::shared_ptr<scene::Scene> m_scene;
    std::shared_ptr<scene::Skybox> m_skybox;

    rendering::renderer::ShaderToModelRenderData models;
};

class App : public starl::application::Application {
    using starl::application::Application::Application;

public:
    virtual void onStart() override {
        initPaths();
        context = std::make_shared<Context>(createApplicationContextResource());
        switchContext(context);
    }

    void initPaths() {
        starl::core::path::PathManager::registerResourcePath<starl::platform::shader::Shader>(SHADERS_DIR);
        starl::core::path::PathManager::registerResourcePath<starl::platform::texture::Texture>(TEXTURES_DIR);
        starl::core::path::PathManager::registerResourcePath<starl::platform::texture::Cubemap>(CUBEMAPS_DIR);
        starl::core::path::PathManager::registerResourcePath<starl::geometry::Model>(MODELS_DIR);
    }

private:
    std::shared_ptr<Context> context;
};

STARLIGHT_ENTRYPOINT(App);
