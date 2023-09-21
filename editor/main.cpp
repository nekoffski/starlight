
#include <starlight/core/Context.h>
#include <starlight/core/event/Event.h>
#include <starlight/core/event/Input.h>
#include <starlight/core/event/Quit.h>
#include <starlight/core/event/WindowResized.h>
#include <starlight/core/utils/Log.h>
#include <starlight/core/window/Keys.h>

#include <starlight/renderer/Geometry.h>
#include <starlight/renderer/RenderPacket.h>
#include <starlight/renderer/Renderer.h>
#include <starlight/renderer/camera/EulerCamera.h>

#include <starlight/core/memory/DynamicAllocator.h>

#include <starlight/resource/ResourceManager.h>

sl::Geometry* getTestUiGeometry(sl::ResourceManager& resourceManager) {
    sl::GeometryProperties2D props;

    props.materialName = "test_ui";
    props.name         = "test_geometry";
    props.indices      = { 2, 1, 0, 3, 0, 1 };

    constexpr float f = 512.0f;

    props.vertices = {
        sl::Vertex2{{ 0.0f, 0.0f }, { 0.0f, 1.0f }},
        sl::Vertex2{ { f, f },      { 1.0f, 0.0f }},
        sl::Vertex2{ { 0.0f, f },   { 0.0f, 0.0f }},
        sl::Vertex2{ { f, 0.0f },   { 1.0f, 1.0f }}
    };

    return resourceManager.loadGeometry(props);
}

int main() {
    sl::Context ctx("starligt-sandbox");

    LOG_INFO("Sandbox starting!");

    bool isRunning = true;

    sl::Renderer renderer(*ctx.getWindow(), *ctx.getConfig());

    auto& eventManager = sl::EventManager::get();

    const auto onQuit = [&]([[maybe_unused]] sl::QuitEvent*) { isRunning = false; };

    const auto onKey = [&](sl::KeyEvent* event) {
        if ((event->key == SL_KEY_ESCAPE || event->key == SL_KEY_Q) && event->action == sl::KeyAction::press)
            isRunning = false;
    };

    const auto onWindowResized = [&](sl::WindowResized* event) {
        renderer.resizeViewport(event->width, event->height);
    };

    eventManager.on<sl::QuitEvent>(onQuit)
      .on<sl::KeyEvent>(onKey)
      .on<sl::WindowResized>(onWindowResized);

    eventManager
      .on<sl::QuitEvent>([&]([[maybe_unused]] sl::QuitEvent*) { isRunning = false; })
      .on<sl::KeyEvent>([&](sl::KeyEvent* event) {
          if ((event->key == SL_KEY_ESCAPE || event->key == SL_KEY_Q) && event->action == sl::KeyAction::press)
              isRunning = false;
      })
      .on<sl::WindowResized>([&](sl::WindowResized* event) {
          renderer.resizeViewport(event->width, event->height);
      });

    sl::ResourceManager resourceManager(
      *renderer.getTextureLoader(), *renderer.getGPUMemoryProxy()
    );

    auto testUiGeometry = getTestUiGeometry(resourceManager);

    // test render data
    sl::RenderPacket packet{};

    sl::GeometryRenderData testRender;
    testRender.geometry = resourceManager.getDefaultGeometry3D();
    testRender.model    = sl::identityMatrix;

    sl::GeometryRenderData testUiRender;
    testUiRender.geometry = testUiGeometry;
    testUiRender.model    = sl::identityMatrix;

    // TODO: am I sure that using spans in packet is a good idea?
    std::vector<sl::GeometryRenderData> geometries   = { testRender };
    std::vector<sl::GeometryRenderData> uiGeometries = { testUiRender };

    packet.geometries   = geometries;
    packet.uiGeometries = uiGeometries;

    sl::EulerCamera camera(sl::EulerCamera::Properties{
      .target = sl::Vec3f{ 0.0f }, .radius = 5.0f });

    while (isRunning) {
        const auto deltaTime = ctx.beginFrame();

        renderer.drawFrame(packet, camera, deltaTime);

        camera.update(deltaTime);
        ctx.endFrame();
    }

    return 0;
}
