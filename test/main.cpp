#include <iostream>

#include <starlight/core/log/Logger.h>
#include <starlight/platform/gpu/GraphicsContext.h>
#include <starlight/platform/input/Input.h>
#include <starlight/platform/window/Window.h>

#include <starlight/platform/shader/ShaderCompiler.hpp>
#include <starlight/platform/shader/Shader.h>

#include <starlight/platform/gpu/VertexBuffer.h>
#include <starlight/platform/gpu/VertexArray.h>
#include <starlight/platform/gpu/RenderAPI.h>

#include <starlight/asset/AssetManager.hpp>
#include <starlight/asset/PathManager.hpp>

using namespace starl::platform;

int main() {
    starl::core::log::Logger::init();
    starl::asset::PathManager::registerResourcePath<shader::Shader>(SHADERS_DIR);

    auto window = window::Window::create(window::WindowParams{
        window::Viewport{ 1600, 900 }, "title" });

    window->init();

    auto ctx = gpu::GraphicsContext::create(window->getHandle());
    ctx->init();

    auto input = input::Input::create(window->getHandle());

    starl::asset::AssetManager assetManager;
    shader::ShaderCompiler::init();

    auto shader = assetManager.load<shader::Shader>({ "/t.vert", "/t.frag", "" });

    try {
        shader::ShaderCompiler::compile(shader);
    } catch (PlatformException& e) {
        std::cout << e;
        return -1;
    }

    auto renderAPI = gpu::RenderAPI::create();

    shader->enable();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    auto buffer = gpu::VertexBuffer::create(vertices, sizeof(vertices));
    buffer->addMemoryOffsetScheme(3, GL_FLOAT, sizeof(float));
    buffer->addMemoryOffsetScheme(3, GL_FLOAT, sizeof(float));

    auto vao = gpu::VertexArray::create();
    vao->addVertexBuffer(buffer);

    vao->bind();
    while (!window->getShouldClose()) {
        ctx->clearBuffers();

        window->update(1.0f);
        renderAPI->clearColor(.5f, .3f, .1f, 1.0f);
        renderAPI->drawArrays(GL_TRIANGLES, 0, 3);

        ctx->swapBuffers();
        if (input->isKeyPressed(STARL_KEY_ESCAPE)) {
            window->setShouldClose(true);
        }
    }

    shader->disable();

    return 0;
}
