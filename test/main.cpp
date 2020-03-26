#include <iostream>

#include <starlight/core/log/Logger.h>
#include <starlight/platform/gpu/GraphicsContext.h>
#include <starlight/platform/input/Input.h>
#include <starlight/platform/window/Window.h>

#include <starlight/platform/shader/Shader.h>
#include <starlight/platform/shader/ShaderCompiler.hpp>

#include <starlight/platform/gpu/RenderAPI.h>
#include <starlight/platform/gpu/VertexArray.h>
#include <starlight/platform/gpu/VertexBuffer.h>

#include <starlight/platform/clock/Clock.h>

#include <starlight/asset/AssetManager.hpp>
#include <starlight/asset/PathManager.hpp>

#include <starlight/framework/graphics/LowLevelRenderer.h>
#include <starlight/framework/graphics/camera/EulerCamera.h>

#include <starlight/rendering/renderer/ModelRenderer.h>
#include <starlight/rendering/renderer/CubemapRenderer.h>

using namespace starl::platform;
using namespace starl::framework;
using namespace starl;

int main() {
    starl::core::log::initLogging();
    starl::asset::PathManager::registerResourcePath<shader::Shader>(SHADERS_DIR);
    starl::asset::PathManager::registerResourcePath<texture::Texture>(TEXTURES_DIR);
    starl::asset::PathManager::registerResourcePath<texture::Cubemap>(CUBEMAPS_DIR);

    static auto logger = core::log::createLogger("main");

    auto window = window::Window::create(window::WindowParams{
        window::Viewport{ 1600, 900 }, "title" });

    window->init();

    auto input = input::Input::create(window->getHandle());

    starl::framework::graphics::LowLevelRenderer llrenderer(window);
    llrenderer.init();

    starl::asset::AssetManager assetManager;
    shader::ShaderCompiler::init();

    auto shader = assetManager.load<shader::Shader>({ "/t.vert", "/t.frag", "" });
    auto cubemapShader = assetManager.load<shader::Shader>({ "/cubemap.vert", "/cubemap.frag" });
    auto texture = assetManager.load<texture::Texture>({ "/wall.jpg" });
    auto cubemap = assetManager.load<texture::Cubemap>({
        "/skybox/posx.jpg",
        "/skybox/negx.jpg",
        "/skybox/posy.jpg",
        "/skybox/negy.jpg",
        "/skybox/posz.jpg",
        "/skybox/negz.jpg"
    });

    try {
        shader::ShaderCompiler::compile(shader);
        shader::ShaderCompiler::compile(cubemapShader);
    } catch (PlatformException& e) {
        std::cout << e;
        return -1;
    }

    float vertices[] = {
        -0.5f, -0.5f, -1.0f, 0.0f, 0.0,
        0.5f, -0.5f, -1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, -1.0f, 0.5f, 1.0f
    };

    auto buffer = gpu::VertexBuffer::create(vertices, sizeof(vertices), 3);
    buffer->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));
    buffer->addMemoryOffsetScheme(2, STARL_FLOAT, sizeof(float));

    auto vao = gpu::VertexArray::create();
    vao->addVertexBuffer(buffer);

    auto camera = graphics::camera::EulerCamera::create(glm::vec3(0.0f), 1.0f, 3.0f);
    starl::rendering::renderer::ModelRenderer modelRenderer(llrenderer);

    auto model = std::make_shared<geometry::Model>();
    auto mesh = std::make_shared<geometry::Mesh>();

    mesh->vertexArray = vao;
    mesh->textures.push_back(texture);
    model->meshes.push_back(mesh);

    auto modelRenderObject = std::make_shared<rendering::renderable::ModelRenderObject>();
    modelRenderObject->model = model;
    modelRenderObject->modelMatrix = std::make_shared<math::Mat4>(1.0f);

    modelRenderer.pushModelRenderObject(shader, modelRenderObject);
    modelRenderer.setCamera(camera);

    clock::Clock clock;

    // cubemap
    rendering::renderer::CubemapRenderer cubemapRenderer(llrenderer);
    cubemapRenderer.setCamera(camera);
    cubemapRenderer.setCubemap(cubemap);
    cubemapRenderer.setCubemapShader(cubemapShader);

    logger->trace("Starting main loop");
    while (!window->getShouldClose()) {
        const auto delta = clock.getDeltaTime();

        window->update(delta);
        camera->update(delta);

        llrenderer.begin();
        modelRenderer.render();
        cubemapRenderer.render();
        llrenderer.end();

        camera->handleInput(input);

        if (input->isKeyPressed(STARL_KEY_ESCAPE)) {
            window->setShouldClose(true);
        }

        clock.update();
    }
    logger->trace("Main loop finished");

    return 0;
}
