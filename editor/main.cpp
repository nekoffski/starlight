
#include <starlight/core/Context.h>
#include <starlight/core/event/Event.h>
#include <starlight/core/event/Input.h>
#include <starlight/core/event/Quit.h>
#include <starlight/core/event/WindowResized.h>
#include <starlight/core/utils/Log.h>
#include <starlight/core/window/Keys.h>

#include <starlight/renderer/Geometry.h>
#include <starlight/renderer/RenderPacket.h>
#include <starlight/renderer/RendererFrontend.h>
#include <starlight/renderer/camera/EulerCamera.h>
#include <starlight/renderer/camera/FirstPersonCamera.h>
#include <starlight/renderer/Mesh.h>

#include <starlight/core/memory/DynamicAllocator.h>
#include <starlight/resource/ResourceManager.h>
#include <starlight/resource/resources/MeshConfig.h>
#include <starlight/renderer/ui/UI.hpp>

std::vector<sl::Mesh> meshes;

void generateMeshes(sl::ResourceManager& resourceManager) {
    sl::Mesh cubeMesh1;
    auto props1 = sl::GeometryConfig3D::generateCube(sl::CubeProperties{
      10.0f, 10.0f, 10.0f, 1, 1, "test_cube", "Builtin.Material.Test" });
    props1.generateTangents();
    cubeMesh1.geometries.push_back(resourceManager.loadGeometry(props1));

    sl::Mesh cubeMesh2;
    auto props2 = sl::GeometryConfig3D::generateCube(sl::CubeProperties{
      5.0f, 5.0f, 5.0f, 1, 1, "test_cube2", "Builtin.Material.Test" });
    props2.generateTangents();
    cubeMesh2.geometries.push_back(resourceManager.loadGeometry(props2));
    cubeMesh2.transform.translate(sl::Vec3f(10.0f, 0.0f, 1.0f));

    sl::Mesh cubeMesh3;
    auto props3 = sl::GeometryConfig3D::generateCube(sl::CubeProperties{
      2.0f, 2.0f, 2.0f, 1, 1, "test_cube3", "Builtin.Material.Test" });
    props3.generateTangents();
    cubeMesh3.geometries.push_back(resourceManager.loadGeometry(props3));
    cubeMesh3.transform.translate(sl::Vec3f(5.0f, 0.0f, 1.0f));

    // meshes.push_back(cubeMesh1);
    // meshes.push_back(cubeMesh2);
    // meshes.push_back(cubeMesh3);

    // Loaded mesh
    sl::Mesh mesh;

    // auto config = sl::MeshConfig::loadOBJ("sponza");
    auto config = sl::MeshConfig::loadOBJ("falcon");
    for (auto& material : config->materials) resourceManager.loadMaterial(material);
    for (auto& geometry : config->geometries)
        mesh.geometries.push_back(resourceManager.loadGeometry(geometry));
    // mesh.transform.translate(sl::Vec3f{ 0.0, -1.0f, 0.0f }).scale(0.05f);

    meshes.push_back(mesh);

    meshes[1].transform.setParent(&meshes[0].transform);
    meshes[2].transform.setParent(&meshes[1].transform);
}

sl::Geometry* getTestUiGeometry(sl::ResourceManager& resourceManager) {
    sl::GeometryConfig2D props;

    props.materialName = "Builtin.Material.UI";
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

sl::RenderPacket getRenderPacket(sl::ResourceManager& resourceManager) {
    auto testUiGeometry = getTestUiGeometry(resourceManager);

    // test render data
    sl::RenderPacket packet{};

    sl::GeometryRenderData testUiRender;
    testUiRender.geometry = testUiGeometry;
    testUiRender.model    = sl::identityMatrix;

    packet.uiGeometries = { testUiRender };

    return packet;
}

bool s_update = true;

void update(sl::RenderPacket& packet, float dt) {
    packet.geometries.clear();

    const auto rotationAxis = sl::Vec3f{ 0.0f, 1.0f, 0.0f };
    const auto angle        = 0.1f * dt;

    if (s_update) {
        meshes[0].transform.rotate(rotationAxis, angle);
    }
    // meshes[1].transform.rotate(rotationAxis, angle);
    // meshes[2].transform.rotate(rotationAxis, angle);

    for (auto& mesh : meshes) {
        for (auto& geometry : mesh.geometries) {
            sl::GeometryRenderData data;
            data.geometry = geometry;
            data.model    = mesh.transform.getWorld();

            packet.geometries.push_back(data);
        }
    }
}

void renderUI(float deltaTime, sl::FrameStatistics& stats) {
    static bool active = true;
    ImGui::Begin("Starlight debug", &active, ImGuiWindowFlags_MenuBar);
    ImGui::Text("Frame time: %f", deltaTime);
    ImGui::Text("Frames per second: %d", int(1.0f / deltaTime));
    ImGui::Text("Frame number: %lu", stats.frameNumber);
    ImGui::Separator();
    ImGui::Text("Rendered vertices: %lu", stats.renderedVertices);

    ImGui::End();
}

// TODO: I know that this is a mess, at some point I'll redesign the outer interface
int main() {
    meshes.reserve(10);

    sl::Context ctx("starligt-sandbox");

    LOG_INFO("Sandbox starting!");

    bool isRunning = true;

    auto window       = ctx.getWindow();
    const auto [w, h] = window->getSize();

    sl::EulerCamera eulerCamera(sl::EulerCamera::Properties{
      .target         = sl::Vec3f{ 0.0f },
      .radius         = 5.0f,
      .viewportWidth  = w,
      .viewportHeight = h,
    });
    sl::FirstPersonCamera firstPersonCamera(sl::FirstPersonCamera::Properties{
      .position       = sl::Vec3f{ 0.0f },
      .viewportWidth  = w,
      .viewportHeight = h,
    });

    std::string renderMode = "standard";

    sl::Camera* currentCamera = &eulerCamera;

    sl::RendererFrontend rendererFrontend(*window, *ctx.getConfig(), currentCamera);

    auto& eventManager = sl::EventManager::get();

    const auto onQuit = [&]([[maybe_unused]] sl::QuitEvent*) { isRunning = false; };

    const auto onKey = [&](sl::KeyEvent* event) {
        if (event->action == sl::KeyAction::press) {
            const auto& key = event->key;

            if ((key == SL_KEY_ESCAPE || key == SL_KEY_Q)) isRunning = false;

            if (key == SL_KEY_0) {
                rendererFrontend.setRenderMode(sl::RenderMode::lights);
                renderMode = "lights only";
            }
            if (key == SL_KEY_9) {
                rendererFrontend.setRenderMode(sl::RenderMode::normals);
                renderMode = "normals only";
            }
            if (key == SL_KEY_8) {
                rendererFrontend.setRenderMode(sl::RenderMode::standard);
                renderMode = "standard";
            }
            if (key == SL_KEY_6) sl::enableVariableLogging();
            if (key == SL_KEY_4) {
                currentCamera = &eulerCamera;
                rendererFrontend.setCamera(currentCamera);
                ctx.getWindow()->showCursor();
            }
            if (key == SL_KEY_3) {
                currentCamera = &firstPersonCamera;
                rendererFrontend.setCamera(currentCamera);
                ctx.getWindow()->hideCursor();
            }
            if (key == SL_KEY_U) s_update = !s_update;
        }
    };

    const auto onWindowResized = [&](sl::WindowResized* event) {
        rendererFrontend.onViewportResize(event->width, event->height);
        eulerCamera.onViewportResize(event->width, event->height);
        firstPersonCamera.onViewportResize(event->width, event->height);
    };

    eventManager.on<sl::QuitEvent>(onQuit)
      .on<sl::KeyEvent>(onKey)
      .on<sl::WindowResized>(onWindowResized);

    sl::ResourceManager resourceManager(*rendererFrontend.getResourcePools());

    rendererFrontend.setCoreShaders(
      resourceManager.getUIDefaultShader(),
      resourceManager.getMaterialDefaultShader()
    );

    generateMeshes(resourceManager);
    auto packet = getRenderPacket(resourceManager);

    sl::FrameStatistics stats;

    while (isRunning) {
        const auto deltaTime = ctx.beginFrame();

        LOG_VAR(deltaTime);
        LOG_VAR(currentCamera->getPosition());

        rendererFrontend.addMainPass(packet);
        rendererFrontend.addUIPass([&]() { renderUI(deltaTime, stats); });
        stats = rendererFrontend.renderFrame(deltaTime);

        update(packet, deltaTime);

        currentCamera->update(deltaTime);
        ctx.endFrame();

        // break;
    }

    return 0;
}
