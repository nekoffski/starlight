#include "Application.h"

#include "starlight/core/event/Quit.h"
#include "starlight/core/event/WindowResized.h"
#include "starlight/core/event/Input.h"

#include "starlight/renderer/views/UIRenderView.h"
#include "starlight/renderer/views/WorldRenderView.h"
#include "starlight/renderer/views/SkyboxRenderView.h"
#include "starlight/renderer/RenderPacket.h"

#include "starlight/ui/UI.hpp"
#include "starlight/ui/fonts/FontAwesome.h"

Application::Application(int argc, char** argv) :
    m_isRunning(false), m_update(false), m_context("Starlight Editor"),
    m_window(m_context.getWindow()), m_renderer(*m_window, *m_context.getConfig()),
    m_resourceManager(*m_renderer.getResourcePools()), m_activeCamera(nullptr) {
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

    auto mesh = m_resourceManager.loadMesh("falcon");
    // mesh->transform.translate(sl::Vec3f{ 0.0, -1.0f, 0.0f }).scale(0.05f);
    m_meshes.push_back(*mesh);

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

    sl::UIRenderView uiView(m_activeCamera, uiProperties, [&](float delta) {
        renderUI(delta);
    });

    auto materialShader = m_resourceManager.loadShader("Builtin.Shader.Material");
    sl::WorldRenderView worldView{ m_activeCamera, materialShader };

    auto skyboxShader = m_resourceManager.loadShader("Builtin.Shader.Skybox");
    auto skybox       = m_resourceManager.loadSkybox("skybox/skybox", *skyboxShader);

    ASSERT(skybox, "Could not load skybox");
    sl::SkyboxRenderView skyboxView{ m_activeCamera, skyboxShader, skybox.get() };

    m_views.push_back(&skyboxView);
    m_views.push_back(&worldView);
    m_views.push_back(&uiView);

    LOG_INFO("Starlight Editor starting");

    m_isRunning = true;
    m_renderer.init(m_views);

    while (m_isRunning) {
        m_context.beginFrame([&](float deltaTime) {
            LOG_VAR(deltaTime);
            LOG_VAR(m_activeCamera->getPosition());

            sl::RenderPacket renderPacket{ m_meshes };
            m_frameStatistics = m_renderer.renderFrame(deltaTime, renderPacket);

            if (m_update) updateScene(deltaTime);
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

            if ((key == SL_KEY_ESCAPE || key == SL_KEY_Q)) m_isRunning = false;

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

    const auto onWindowResized = [&](sl::WindowResized* event) {
        const auto& [w, h] = *event;
        m_renderer.onViewportResize(w, h);
        m_eulerCamera->onViewportResize(w, h);
        m_firstPersonCamera->onViewportResize(w, h);
    };

    sl::EventManager::get()
      .on<sl::QuitEvent>(onQuit)
      .on<sl::KeyEvent>(onKey)
      .on<sl::WindowResized>(onWindowResized);
}

void Application::renderUI(float delta) {
    float menuHeight = 0.0f;

    if (ImGui::BeginMainMenuBar()) {
        menuHeight = ImGui::GetWindowSize().y;

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Create")) {
            }
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
            }
            if (ImGui::MenuItem("Save as..")) {
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    const auto [w, h] = m_window->getSize();

    float heightLeft = h - menuHeight;
    sl::ui::PanelCombo::Properties leftComboProperties{
        .position = {0,         menuHeight},
          .size = { 0.2f * w, heightLeft}
    };
    sl::ui::PanelCombo leftCombo{ "left-combo", leftComboProperties };

    leftCombo
      .addPanel(
        "Statistics",
        [&]() {
            ImGui::Text("Frame time: %f", delta);
            ImGui::Text("Frames per second: %d", int(1.0f / delta));
            ImGui::Text("Frame number: %lu", m_frameStatistics.frameNumber);
            ImGui::Separator();
            ImGui::Text(
              "Rendered vertices: %lu", m_frameStatistics.renderedVertices
            );
            ImGui::Separator();
            ImGui::Text("8/9/0 - change render mode");
            ImGui::Text("6     - dump var logs");
            ImGui::Text("3/4   - switch camera");
            ImGui::Text("u     - on/off update");
        }
      )
      .addPanel("Scene", []() { ImGui::Text("Just a placeholder for now"); });
    leftCombo.render();
}

void Application::updateScene(float delta) {
    const auto rotationAxis = sl::Vec3f{ 0.0f, 1.0f, 0.0f };
    const auto angle        = 0.1f * delta;
    m_meshes[0].transform.rotate(rotationAxis, angle);
}
