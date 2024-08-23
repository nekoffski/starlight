

#include <csignal>

#include "starlight/core/Core.hh"
#include "starlight/core/window/Window.hh"
#include "starlight/core/Context.hh"

#include "starlight/renderer/views/WorldRenderView.hh"
#include "starlight/renderer/camera/EulerCamera.hh"
#include "starlight/renderer/views/SkyboxRenderView.hh"
#include "starlight/renderer/RendererFrontend.hh"

#include "starlight/renderer/gpu/Shader.hh"

static std::atomic_bool isRunning = true;

int main() {
    std::signal(SIGINT, []([[maybe_unused]] int) { isRunning = false; });

    sl::Context context;
    sl::RendererFrontend renderer{ context };

    auto& window            = context.getWindow();
    const auto viewportSize = window.getFramebufferSize();

    sl::EulerCamera camera(sl::EulerCamera::Properties{
      .target       = sl::Vec3<sl::f32>{ 0.0f },
      .radius       = 5.0f,
      .viewportSize = viewportSize,
    });

    auto& rendererBackend = renderer.getRendererBackend();

    auto skybox = sl::Skybox::load("skybox2/skybox");

    sl::SkyboxRenderView skyboxView{ &camera, skybox.get() };
    std::vector<sl::RenderView*> renderViews{ &skyboxView };

    // sus, RendererPipeline instead?
    renderer.init(renderViews);

    sl::RenderPacket renderPacket;

    renderPacket.viewport = sl::Rect2<sl::u32>{
        sl::Vec2<sl::u32>{0u, 0u},
        viewportSize
    };

    while (isRunning) {
        context.beginFrame([&](float deltaTime) {
            renderer.renderFrame(deltaTime, renderPacket);
            camera.update(deltaTime);
        });
    }

    return 0;
}
