#pragma once

#include <vector>

#include "starlight/core/Core.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/window/Window.h"
#include "starlight/core/Context.h"
#include "starlight/core/FileSystem.h"

#include "starlight/renderer/camera/EulerCamera.h"
#include "starlight/renderer/camera/FirstPersonCamera.h"
#include "starlight/renderer/RendererFrontend.h"
#include "starlight/renderer/views/RenderView.h"

#include "starlight/resource/ResourceContext.h"

#include "starlight/scene/Scene.h"
#include "starlight/scene/SceneSerializer.h"
#include "starlight/scene/SceneDeserializer.h"

#include "ui/UI.h"

class Application {
public:
    explicit Application(int argc, char** argv);

    int run();

private:
    void setupEventHandlers();
    void calculateViewport();

    sl::Viewport m_viewport;

    bool m_isRunning;
    bool m_update;

    sl::Context m_context;
    sl::Window& m_window;
    sl::RendererFrontend m_renderer;
    sl::ResourceContext m_resourceContext;

    UI m_ui;
    Logger* m_logger;

    sl::UniqPtr<sl::EulerCamera> m_eulerCamera;
    sl::UniqPtr<sl::FirstPersonCamera> m_firstPersonCamera;
    sl::Camera* m_activeCamera;

    sl::FileSystem m_fileSystem;

    sl::Scene m_scene;
    sl::SceneSerializer m_sceneSerializer;
    sl::SceneDeserializer m_sceneDeserializer;

    std::vector<sl::RenderView*> m_views;
    sl::FrameStatistics m_frameStatistics;
};
