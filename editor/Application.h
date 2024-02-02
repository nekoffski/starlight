#pragma once

#include <vector>

#include "starlight/core/Core.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/window/Window.h"
#include "starlight/core/Context.h"

#include "starlight/renderer/camera/EulerCamera.h"
#include "starlight/renderer/camera/FirstPersonCamera.h"
#include "starlight/renderer/RendererFrontend.h"
#include "starlight/renderer/views/RenderView.h"
#include "starlight/renderer/Mesh.h"

#include "starlight/resource/ResourceManager.h"

#include "starlight/scene/Scene.h"

#include "UI.h"

class Application {
public:
    explicit Application(int argc, char** argv);

    int run();

private:
    void setupEventHandlers();
    void updateScene(float delta);

    bool m_isRunning;
    bool m_update;

    sl::Context m_context;
    sl::Window* m_window;
    sl::RendererFrontend m_renderer;
    sl::ResourceManager m_resourceManager;

    UI m_ui;

    sl::UniqPtr<sl::EulerCamera> m_eulerCamera;
    sl::UniqPtr<sl::FirstPersonCamera> m_firstPersonCamera;
    sl::Camera* m_activeCamera;

    sl::Scene m_scene;

    std::vector<sl::RenderView*> m_views;
    std::vector<sl::Mesh> m_meshes;

    sl::FrameStatistics m_frameStatistics;
};
