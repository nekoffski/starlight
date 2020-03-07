
#include <starlight/core/log/Logger.h>
#include <starlight/platform/gpu/GraphicsContext.h>
#include <starlight/platform/input/Input.h>
#include <starlight/platform/window/Window.h>

using namespace starl::platform;

int main() {
    starl::core::log::Logger::init();

    auto window = window::Window::create(window::WindowParams{
        window::Viewport{ 1600, 900 }, "title" });

    window->init();

    auto ctx = gpu::GraphicsContext::create(window->getHandle());
    auto input = input::Input::create(window->getHandle());

    while (!window->getShouldClose()) {
        window->update(1.0f);
        ctx->swapBuffers();

        if (input->isKeyPressed(STARL_KEY_ESCAPE)) {
            window->setShouldClose(true);
        }
    }

    return 0;
}