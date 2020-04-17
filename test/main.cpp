#include <starlight/application/Entrypoint.hpp>
#include <starlight/application/context/ApplicationContext.h>
#include <starlight/core/path/PathManager.hpp>
#include <starlight/framework/graphics/camera/EulerCamera.h>
#include <starlight/geometry/Model.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/platform/texture/Cubemap.h>
#include <starlight/platform/texture/Texture.h>

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
        m_applicationContextResource.cubemapRenderer->setCubemap(m_cubemap);
        m_applicationContextResource.cubemapRenderer->setCubemapShader(m_cubemapShader);
        m_applicationContextResource.modelRenderer->setCamera(m_camera);

        m_shader = m_applicationContextResource.assetManager.load<shader::Shader>({ "/t.vert", "/t.frag", "" });
        m_texture = m_applicationContextResource.assetManager.load<texture::Texture>({ "/wall.jpg" });
        m_model = m_applicationContextResource.assetManager.load<geometry::Model>({ "/tow/tower.obj" });
        m_modelRenderEntity = std::make_shared<rendering::entity::ModelRenderEntity>(m_model);

        entities.insert({ m_shader, { m_modelRenderEntity } });
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
    }

    void render() override {
        m_applicationContextResource.modelRenderer->render(entities);
        m_applicationContextResource.cubemapRenderer->render();
    }

private:
    std::shared_ptr<platform::shader::Shader> m_cubemapShader;
    std::shared_ptr<platform::shader::Shader> m_shader;
    std::shared_ptr<platform::texture::Cubemap> m_cubemap;
    std::shared_ptr<framework::graphics::camera::UserControllableCamera> m_camera;
    std::shared_ptr<platform::texture::Texture> m_texture;
    std::shared_ptr<geometry::Model> m_model;
    std::shared_ptr<rendering::entity::ModelRenderEntity> m_modelRenderEntity;

    rendering::renderer::ShaderToModelRenderEntities entities;
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
