#pragma once

#include "DebugConsole.hpp"
#include "EngineMode.h"
#include "EngineState.h"
#include "Events.h"
#include "gui/EditorGui.h"
#include "sl/app/ApplicationContext.h"
#include "sl/app/Deserializer.h"
#include "sl/app/Serializer.h"
#include "sl/asset/AssetManager.h"
#include "sl/core/BaseError.hpp"
#include "sl/core/FileSystem.h"
#include "sl/core/InputManager.h"
#include "sl/ecs/Entity.h"
#include "sl/event/Categories.h"
#include "sl/event/Event.h"
#include "sl/gfx/ViewFrustum.h"
#include "sl/gfx/camera/EulerCamera.h"
#include "sl/gfx/camera/FPSCamera.h"
#include "sl/gui/ErrorDialog.hpp"

#include "sl/gui/Utils.hpp"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/scene/Scene.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"
#include <kc/core/Log.h>

#include "sl/rendering/CustomFrameBufferRenderPass.h"
#include "sl/rendering/DefaultFrameBufferRenderPass.h"
#include "sl/rendering/RenderPass.h"
#include "sl/rendering/RenderPipeline.h"
#include "sl/rendering/stages/BlurColorBufferStage.h"
#include "sl/rendering/stages/CaptureDirectionalDepthMapsStage.h"
#include "sl/rendering/stages/CapturePointDepthMapsStage.h"
#include "sl/rendering/stages/PrepareLightsStage.h"
#include "sl/rendering/stages/RenderBoundingBoxesStage.h"
#include "sl/rendering/stages/RenderColorBufferState.h"
#include "sl/rendering/stages/RenderMeshesStage.h"
#include "sl/rendering/stages/RenderSkyboxStage.h"
#include "sl/rendering/stages/RenderVectorsStage.h"

#include "sl/gfx/RenderBuffer.h"

#include <memory>

using namespace sl;
using namespace sl::scene;
using namespace sl::core;

const math::Vec3 sceneOrigin = { 0.0f, 0.0f, 0.0f };

class EditorContext : public app::ApplicationContext {
public:
    explicit EditorContext(const std::string& ident)
        : ApplicationContext(ident)
        , m_engineState(editor::EngineState::stopped)
        , m_depthFrameBuffer(gfx::FrameBuffer::factory->create())
        , m_captureDepthMapsRenderPass(m_depthFrameBuffer.get())
        , m_sceneQuadFrameBuffer(gfx::FrameBuffer::factory->create())
        , m_captureSceneRenderPass(m_sceneQuadFrameBuffer.get()) {
    }

    void onInit() override {
        auto [windowWidth, windowHeight] = m_windowProxy->getSize();

        auto viewFrustum = gfx::ViewFrustum { windowWidth, windowHeight };
        m_activeCamera = std::make_shared<gfx::camera::EulerCamera>(viewFrustum, math::Vec3(0.0f), 1.0f, 8.0f);
        m_scene = scene::Scene::create();

        // TODO: remove!
        auto& viewport = m_activeCamera->viewFrustum.viewport;
        auto guiSharedState = std::make_shared<editor::gui::SharedState>(m_assetManager, viewport.width, viewport.height);
        m_editorGui = std::make_shared<editor::gui::EditorGui>(guiSharedState);

        m_scene->camera = m_activeCamera;
        m_editorGui->sharedState->activeScene = m_scene;

        m_engineMode = editor::EngineMode::inEditor;

        recalculateViewportSize(windowWidth, windowHeight);

        // setup rendering pipeline
        m_renderColorBufferStage.setWindowProxy(m_windowProxy.get());
        m_renderColorBufferStage.setColorBuffer(m_colorBuffer.get());

        m_blurColorBufferStage.setWindowProxy(m_windowProxy.get());

        m_captureDepthMapsRenderPass
            .addRenderStage(&m_captureDirectionalDepthMapsStage)
            .addRenderStage(&m_capturePointDepthMapsStage);

        // m_finalRenderPass
        m_captureSceneRenderPass
            .addRenderStage(&m_prepareLightsStage)
            .addRenderStage(&m_renderMeshesStage)
            .addRenderStage(&m_renderBoundingBoxesStage)
            .addRenderStage(&m_renderVectorsStage)
            .addRenderStage(&m_renderSkyboxStage);

        m_bloomPass.addRenderStage(&m_blurColorBufferStage);

        m_finalRenderPass.addRenderStage(&m_renderColorBufferStage);

        m_renderPipeline
            .addRenderPass(&m_captureDepthMapsRenderPass)
            .addRenderPass(&m_captureSceneRenderPass)
            .addRenderPass(&m_bloomPass)
            .addRenderPass(&m_finalRenderPass);

        WRITE_DEBUG("{}", "Editor context initialized");
    }

    void onAttach() override {
    }

    void onDetach() override {
    }

    void renderGui() override {
        if (m_engineMode == editor::EngineMode::inEditor) {
            m_editorGui->renderEditorGui();
            m_errorDialog.show();
        }
    }

    void update(app::SceneSystems& sceneSystems, float deltaTime, float time) override {
        m_activeCamera->update(deltaTime);
        auto pfxs = m_scene->ecsRegistry.getComponentView<components::ParticleEffectComponent>();
        sceneSystems.pfxEngine.update(pfxs, deltaTime, *m_scene->camera);

        if (m_engineState == editor::EngineState::started) {
            using namespace sl::scene::components;

            auto [rigidBodies, transforms] = m_scene->ecsRegistry.getComponentsViews<RigidBodyComponent, TransformComponent>();
            sceneSystems.physxEngine.processRigidBodies(rigidBodies, transforms, deltaTime);
        }

        if (m_engineMode == editor::EngineMode::inGame && core::InputManager::get()->isKeyPressed(STARL_KEY_ESCAPE)) {
            m_engineMode = editor::EngineMode::inEditor;

            auto [width, height] = m_windowProxy->getSize();
            recalculateViewportSize(width, height);
        }
    }

    void render(gfx::Renderer& renderer) override {
        m_renderPipeline.run(renderer, *m_scene);
    }

    void handleEvents(const xvent::EventProvider& eventProvider) override {
        auto events = eventProvider.getByCategories<event::CoreCategory, event::EditorCategory>();

        using namespace sl::event;

        for (auto& event : events) {
            LOG_INFO("Processing event: {}", event->asString());

            if (event->is<SetSkyboxEvent>()) {
                auto cubemap = event->as<SetSkyboxEvent>()->cubemap;
                m_scene->skybox = sl::scene::Skybox { utils::Globals::get()->shaders->defaultCubemapShader, cubemap };

            } else if (event->is<QuitEvent>()) {
                m_windowProxy->quit();

            } else if (event->is<WindowResizedEvent>()) {
                auto windowResizedEvent = event->as<WindowResizedEvent>();
                auto [width, height] = windowResizedEvent->getSize();

                recalculateViewportSize(width, height);

            } else if (event->is<ChangeSceneCenterEvent>()) {
                auto& newCenter = event->as<ChangeSceneCenterEvent>()->center;
                auto sceneCamera = std::dynamic_pointer_cast<sl::gfx::camera::EulerCamera>(m_activeCamera);
                if (sceneCamera)
                    sceneCamera->setCenter(newCenter);

            } else if (event->is<editor::EngineStateChanged>()) {
                handleStateChange(
                    event->as<editor::EngineStateChanged>()->state);

            } else if (event->is<editor::EnterGameMode>()) {
                m_engineMode = editor::EngineMode::inGame;

                auto [width, height] = m_windowProxy->getSize();
                recalculateViewportSize(width, height);
            } else if (event->is<DisplayErrorEvent>()) {
                m_errorDialog.setErrorMessage(event->as<DisplayErrorEvent>()->message);
            }

            try {
                using namespace sl::app;

                if (event->is<SerializeSceneEvent>()) {
                    auto& path = event->as<SerializeSceneEvent>()->path;

                    LOG_INFO("Serializing scene as: {}", path);
                    Serializer { path }.serialize(m_assetManager, m_scene);
                }

                if (event->is<DeserializeSceneEvent>()) {
                    auto& path = event->as<DeserializeSceneEvent>()->path;

                    LOG_INFO("Deserializing scene from: {}", path);
                    Deserializer { m_assetManager, m_scene }.deserialize(path);
                }

            } catch (sl::core::Error& err) {
                m_errorDialog.setErrorMessage(err.as<std::string>());
            }
        }
    }

    void recalculateViewportSize(float width, float height) {
        editor::gui::GuiProperties guiProperties {
            static_cast<int>(width), static_cast<int>(height)
        };
        m_editorGui->sharedState->guiProperties = guiProperties;

        gfx::ViewFrustum::Viewport newViewport;

        if (m_engineMode == editor::EngineMode::inEditor) {
            newViewport.width = static_cast<int>(width - guiProperties.scenePanelProperties.size.x - guiProperties.rightPanelProperties.size.x);
            newViewport.height = static_cast<int>(height - guiProperties.bottomPanelProperties.size.y);
            newViewport.beginX = static_cast<int>(guiProperties.scenePanelProperties.size.x);
            newViewport.beginY = static_cast<int>(guiProperties.bottomPanelProperties.size.y);
        } else {
            newViewport.width = static_cast<int>(width);
            newViewport.height = static_cast<int>(height);
            newViewport.beginX = 0;
            newViewport.beginY = 0;
        }

        m_activeCamera->viewFrustum.viewport = newViewport;
        m_activeCamera->calculateProjectionMatrix();

        m_depthBuffer = gfx : RenderBuffer::factory->create(STARL_DEPTH_COMPONENT, width, height);

        m_colorBuffer = gfx::Texture::factory->create(width, height, STARL_RGBA16F, STARL_RGBA);
        m_bloomBuffer = gfx::Texture::factory->create(width, height, STARL_RGBA16F, STARL_RGBA);

        m_sceneQuadFrameBuffer->bind();
        m_sceneQuadFrameBuffer->bindTexture(*m_colorBuffer, STARL_COLOR_ATTACHMENT0);
        m_sceneQuadFrameBuffer->bindTexture(*m_bloomBuffer, STARL_COLOR_ATTACHMENT1);
        m_sceneQuadFrameBuffer->bindRenderBuffer(*m_depthBuffer);
        m_sceneQuadFrameBuffer->unbind();

        m_blurColorBufferStage.setColorBuffer(m_bloomBuffer.get());

        m_renderColorBufferStage.setColorBuffer(m_colorBuffer.get());
        m_renderColorBufferStage.setBloomBuffer(m_blurColorBufferStage.getOutputColorBuffer());

        m_lowLevelRendererProxy->setViewport(newViewport);
    }

private:
    void handleStateChange(editor::EngineState state) {
        LOG_INFO("Engine state changed to: {}", toString(state));

        m_engineState = state;

        using editor::EngineState;
        using namespace sl::scene::components;

        auto [rigidBodies, transforms] = m_scene->ecsRegistry.getComponentsViews<RigidBodyComponent, TransformComponent>();

        switch (state) {
        case EngineState::started: {
            for (auto& rigidBody : rigidBodies)
                rigidBody.save();

            for (auto& transform : transforms)
                transform.save();

            break;
        }

        case EngineState::stopped: {
            for (auto& rigidBody : rigidBodies)
                rigidBody.restore();

            for (auto& transform : transforms)
                transform.restore();

            break;
        }

        default:
            break;
        }
    }

    sl::gui::ErrorDialog m_errorDialog;

    sl::asset::AssetManager m_assetManager;

    std::shared_ptr<editor::gui::EditorGui> m_editorGui;
    std::shared_ptr<gfx::camera::Camera> m_activeCamera;
    std::shared_ptr<scene::Scene> m_scene;

    editor::EngineState m_engineState;
    editor::EngineMode m_engineMode;

    std::shared_ptr<gfx : FrameBuffer> m_depthFrameBuffer;

    std::shared_ptr<gfx : FrameBuffer> m_sceneQuadFrameBuffer;
    std::unique_ptr<gfx : RenderBuffer> m_depthBuffer;

    std::unique_ptr<gfx::Texture> m_colorBuffer;
    std::unique_ptr<gfx::Texture> m_bloomBuffer;

    rendering::RenderPipeline m_renderPipeline;

    rendering::DefaultFrameBufferRenderPass m_finalRenderPass;
    rendering::DefaultFrameBufferRenderPass m_bloomPass;
    rendering::CustomFrameBufferRenderPass m_captureDepthMapsRenderPass;
    rendering::CustomFrameBufferRenderPass m_captureSceneRenderPass;

    rendering::stages::RenderSkyboxStage m_renderSkyboxStage;
    rendering::stages::RenderMeshesStage m_renderMeshesStage;
    rendering::stages::PrepareLightsStage m_prepareLightsStage;
    rendering::stages::RenderBoundingBoxesStage m_renderBoundingBoxesStage;
    rendering::stages::CaptureDirectionalDepthMapsStage m_captureDirectionalDepthMapsStage;
    rendering::stages::CapturePointDepthMapsStage m_capturePointDepthMapsStage;
    rendering::stages::RenderVectorsStage m_renderVectorsStage;
    rendering::stages::RenderColorBufferStage m_renderColorBufferStage;
    rendering::stages::BlurColorBufferStage m_blurColorBufferStage;
};
