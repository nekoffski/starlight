#include <starlight/application/Entrypoint.hpp>
#include <starlight/core/path/PathManager.hpp>
#include <starlight/geometry/Model.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/platform/texture/Cubemap.h>
#include <starlight/platform/texture/Texture.h>

class App : public starl::application::Application {
    using starl::application::Application::Application;

public:
    virtual void onStart() override {
        starl::core::path::PathManager::registerResourcePath<starl::platform::shader::Shader>(SHADERS_DIR);
        starl::core::path::PathManager::registerResourcePath<starl::platform::texture::Texture>(TEXTURES_DIR);
        starl::core::path::PathManager::registerResourcePath<starl::platform::texture::Cubemap>(CUBEMAPS_DIR);
        starl::core::path::PathManager::registerResourcePath<starl::geometry::Model>(MODELS_DIR);
    }
};

STARLIGHT_ENTRYPOINT(App);

// #include <starlight/core/log/Logger.h>
// #include <starlight/platform/gpu/GraphicsContext.h>
// #include <starlight/platform/input/Input.h>
// #include <starlight/platform/window/Window.h>

// #include <starlight/platform/shader/Shader.h>
// #include <starlight/platform/shader/ShaderCompiler.hpp>

// #include <starlight/platform/gpu/ElementBuffer.h>
// #include <starlight/platform/gpu/RenderAPI.h>
// #include <starlight/platform/gpu/VertexArray.h>
// #include <starlight/platform/gpu/VertexBuffer.h>

// #include <starlight/platform/clock/Clock.h>

// #include <starlight/asset/AssetManager.hpp>
// #include <starlight/core/path/PathManager.hpp>

// #include <starlight/framework/graphics/LowLevelRenderer.h>
// #include <starlight/framework/graphics/camera/EulerCamera.h>
// #include <starlight/platform/model/ModelLoader.h>

// #include <starlight/rendering/renderer/CubemapRenderer.h>
// #include <starlight/rendering/renderer/ModelRenderer.h>
// #include <starlight/scene/Scene.h>

// using namespace starl::platform;
// using namespace starl::framework;
// using namespace starl;

// int main(int argc, char** argv) {
//     starl::core::log::initLogger(argc, argv);
//     starl::core::path::PathManager::registerResourcePath<shader::Shader>(SHADERS_DIR);
//     starl::core::path::PathManager::registerResourcePath<texture::Texture>(TEXTURES_DIR);
//     starl::core::path::PathManager::registerResourcePath<texture::Cubemap>(CUBEMAPS_DIR);
//     starl::core::path::PathManager::registerResourcePath<geometry::Model>(MODELS_DIR);

//     auto window = window::Window::create(window::WindowParams{
//         window::Viewport{ 1600, 900 }, "title" });

//     window->init();

//     auto input = input::Input::create(window->getHandle());

//     starl::framework::graphics::LowLevelRenderer llrenderer(window);
//     llrenderer.init();

//     starl::asset::AssetManager assetManager;
//     shader::ShaderCompiler::init();

//     auto shader = assetManager.load<shader::Shader>({ "/t.vert", "/t.frag", "" });
//     auto cubemapShader = assetManager.load<shader::Shader>({ "/cubemap.vert", "/cubemap.frag" });
//     auto texture = assetManager.load<texture::Texture>({ "/wall.jpg" });
//     auto cubemap = assetManager.load<texture::Cubemap>({ "/skybox/posx.jpg",
//         "/skybox/negx.jpg",
//         "/skybox/posy.jpg",
//         "/skybox/negy.jpg",
//         "/skybox/posz.jpg",
//         "/skybox/negz.jpg" });

//     try {
//         shader::ShaderCompiler::compile(shader);
//         shader::ShaderCompiler::compile(cubemapShader);
//     } catch (PlatformException& e) {
//         LOG(INFO) << e.toStr();
//         return -1;
//     }

//     auto camera = graphics::camera::EulerCamera::create(glm::vec3(0.0f), 1.0f, 8.0f);
//     starl::rendering::renderer::ModelRenderer modelRenderer(llrenderer);

//     modelRenderer.setCamera(camera);

//     clock::Clock clock;

//     // cubemap
//     rendering::renderer::CubemapRenderer cubemapRenderer(llrenderer);
//     cubemapRenderer.setCamera(camera);
//     cubemapRenderer.setCubemap(cubemap);
//     cubemapRenderer.setCubemapShader(cubemapShader);

//     auto model = assetManager.load<geometry::Model>({ "/tow/tower.obj" });
//     auto modelRenderObject = std::make_shared<rendering::entity::ModelRenderEntity>(model);

//     //modelRenderer.pushModelRenderObject(shader, modelRenderObject);

//     scene::Scene scene;

//     rendering::renderer::ShaderToModelRenderEntities m;

//     m.insert({ shader, { modelRenderObject } });

//     while (!window->getShouldClose()) {
//         const auto delta = clock.getDeltaTime();

//         window->update(delta);
//         camera->update(delta);

//         llrenderer.begin();
//         modelRenderer.render(m);
//         cubemapRenderer.render();
//         llrenderer.end();

//         camera->handleInput(input);

//         if (input->isKeyPressed(STARL_KEY_ESCAPE)) {
//             window->setShouldClose(true);
//         }

//         clock.update();
//     }
//     return 0;
// }
