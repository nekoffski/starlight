#include "Application.h"

#include "starlight/core/event/Quit.h"
#include "starlight/core/event/WindowResized.h"
#include "starlight/core/event/Input.h"

#include "starlight/renderer/views/UIRenderView.h"
#include "starlight/renderer/views/WorldRenderView.h"
#include "starlight/renderer/views/SkyboxRenderView.h"
#include "starlight/renderer/RenderPacket.h"

#include "starlight/ui/UI.h"
#include "starlight/ui/fonts/FontAwesome.h"

#include "starlight/scene/components/MeshComponent.h"
#include "starlight/scene/components/TransformComponent.h"

#include "starlight/resource/All.h"

#include "Events.h"

Application::Application(int argc, char** argv) :
    m_isRunning(false), m_update(false), m_context("Starlight Editor"),
    m_window(m_context.getWindow()), m_renderer(*m_window, *m_context.getConfig()),
    m_resourceContext(*m_renderer.getResourcePools()), m_activeCamera(nullptr),
    m_ui(
      m_window->getSize().width,
      m_window->getSize().height,  // TODO: no comment required
      m_renderer, &m_scene
    ),
    m_logger(m_ui.getLogger()), m_sceneSerializer(m_fileSystem) {
    const auto& [w, h] = m_window->getSize();

    m_eulerCamera = sl::createUniqPtr<sl::EulerCamera>(sl::EulerCamera::Properties{
      .target         = sl::Vec3f{ 0.0f },
      .radius         = 5.0f,
      .viewportWidth  = w,
      .viewportHeight = h,
    });
    m_firstPersonCamera =
      sl::createUniqPtr<sl::FirstPersonCamera>(sl::FirstPersonCamera::Properties{
        .position       = sl::Vec3f{ 0.0f },
        .viewportWidth  = w,
        .viewportHeight = h,
      });
    m_activeCamera = m_eulerCamera.get();
}

int Application::run() {
    setupEventHandlers();

    sl::Font::SubfontProperties icons{
        "/home/nek0/kapik/projects/starlight/assets/fonts/fa-solid-900.ttf",
        ICON_MIN_FA, ICON_MAX_FA
    };
    sl::Font::Properties font{
        .name = "main-font",
        .path =
          "/home/nek0/kapik/projects/starlight/assets/fonts/Roboto-Regular.ttf",
        .size     = 15,
        .subfonts = { icons }
    };

    sl::UIRenderView::Properties uiProperties;
    uiProperties.fonts = { font };

    sl::UIRenderView uiView(m_activeCamera, uiProperties, [&]() { m_ui.render(); });

    auto materialShader = sl::ShaderManager::get().load("Builtin.Shader.Material");
    sl::WorldRenderView worldView{ m_activeCamera, materialShader };

    auto skyboxShader = sl::ShaderManager::get().load("Builtin.Shader.Skybox");
    auto skybox = sl::SkyboxManager::get().load("skybox2/skybox", *skyboxShader);

    ASSERT(skybox, "Could not load skybox");
    sl::SkyboxRenderView skyboxView{ m_activeCamera, skyboxShader, skybox };

    m_views.push_back(&skyboxView);
    m_views.push_back(&worldView);
    m_views.push_back(&uiView);

    LOG_INFO("Starlight Editor starting");

    m_isRunning = true;
    m_renderer.init(m_views);

    auto entity1 = m_scene.addEntity("My-Entity");
    auto entity2 = m_scene.addEntity();

    auto model1 = sl::ModelManager::get().load("falcon");
    auto model  = sl::ModelManager::get().load("tower");

    entity1->addComponent<sl::MeshComponent>(nullptr);
    entity1->addComponent<sl::TransformComponent>();

    while (m_isRunning && not m_ui.shouldExit()) {
        m_context.beginFrame([&](float deltaTime) {
            LOG_VAR(deltaTime);
            LOG_VAR(m_activeCamera->getPosition());

            auto renderPacket = m_scene.getRenderPacket();
            m_renderer.renderFrame(deltaTime, renderPacket);

            m_activeCamera->update(deltaTime);
        });
    }

    return 0;
}

void Application::setupEventHandlers() {
    const auto onQuit = [&](sl::QuitEvent* event) {
        LOG_INFO("Received QuitEvent, reason: '{}'", event->reason);
        m_isRunning = false;
    };

    const auto onKey = [&](sl::KeyEvent* event) {
        if (event->action == sl::KeyAction::press) {
            const auto& key = event->key;

            if ((key == SL_KEY_ESCAPE)) m_isRunning = false;
            if (key == SL_KEY_0) m_renderer.setRenderMode(sl::RenderMode::lights);
            if (key == SL_KEY_9) m_renderer.setRenderMode(sl::RenderMode::normals);
            if (key == SL_KEY_8) m_renderer.setRenderMode(sl::RenderMode::standard);
            if (key == SL_KEY_6) sl::enableVariableLogging();
            if (key == SL_KEY_4) {
                m_activeCamera = m_eulerCamera.get();
                m_window->showCursor();
                for (auto& view : m_views) view->setCamera(m_activeCamera);
            }
            if (key == SL_KEY_3) {
                m_activeCamera = m_firstPersonCamera.get();
                m_window->hideCursor();
                for (auto& view : m_views) view->setCamera(m_activeCamera);
            }
            if (key == SL_KEY_U) m_update = !m_update;
        }
    };

    const auto onScroll = [&](sl::ScrollEvent* event) {
        m_eulerCamera->onScroll(event->offset);
    };

    const auto onWindowResized = [&](sl::WindowResized* event) {
        const auto& [w, h] = *event;
        m_renderer.onViewportResize(w, h);
        m_eulerCamera->onViewportResize(w, h);
        m_firstPersonCamera->onViewportResize(w, h);
    };

    const auto onSceneLoaded = [&](SceneLoaded* event) {
        m_logger->info("Loading scene: {}", event->path);
    };

    const auto onSceneSaved = [&](SceneSaved* event) {
        m_logger->info("Saving scene: {}", event->path);
        m_sceneSerializer.serialize(m_scene, event->path);
    };

    sl::EventManager::get()
      .on<sl::QuitEvent>(onQuit)
      .on<sl::KeyEvent>(onKey)
      .on<sl::WindowResized>(onWindowResized)
      .on<sl::ScrollEvent>(onScroll)
      .on<SceneLoaded>(onSceneLoaded)
      .on<SceneSaved>(onSceneSaved);
}
