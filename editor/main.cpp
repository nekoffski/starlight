
#include <starlight/core/Log.h>
#include <starlight/core/Context.h>
#include <starlight/core/event/Event.h>
#include <starlight/core/event/Quit.h>
#include <starlight/core/event/Input.h>
#include <starlight/core/window/Keys.h>
#include <starlight/core/event/WindowResized.h>

#include <starlight/renderer/RenderPacket.h>
#include <starlight/renderer/Renderer.h>
#include <starlight/renderer/camera/EulerCamera.h>

#include <starlight/resource/ResourceManager.h>

int main() {
    sl::initLogging("starlight");
    LOG_INFO("Sandbox starting!");

    sl::Context ctx;

    auto eventManager = sl::EventManager::getPtr();

    bool isRunning = true;

    eventManager->on<sl::QuitEvent>([&](auto* event) { isRunning = false; });
    eventManager->on<sl::KeyEvent>([&](auto* event) {
        if (event->key == SL_KEY_ESCAPE && event->action == sl::KeyAction::press) isRunning = false;
    });

    sl::Renderer renderer(*ctx.getWindow(), *ctx.getConfig());

    eventManager->on<sl::WindowResized>([&](auto* event) {
        renderer.resizeViewport(event->width, event->height);
    });

    sl::ResourceManager resourceManager(
        *renderer.getTextureLoader(), *renderer.getGPUMemoryProxy()
    );

    // test render data
    sl::RenderPacket packet{};

    sl::GeometryRenderData testRender;
    testRender.geometry = resourceManager.getDefaultGeometry();
    testRender.model    = sl::identityMatrix;

    std::vector<sl::GeometryRenderData> geometries = {testRender};

    packet.geometries = geometries;

    sl::EulerCamera camera{
        sl::EulerCamera::Properties{.target = sl::Vec3f{0.0f}, .radius = 5.0f}
    };

    while (isRunning) {
        const auto deltaTime = ctx.beginFrame();

        renderer.drawFrame(packet, camera, deltaTime);

        camera.update(deltaTime);
        ctx.endFrame();
    }

    return 0;
}
