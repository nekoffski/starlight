#pragma once

#include <chrono>
#include <thread>

#include "sl/application/Entrypoint.hpp"
#include "sl/application/context/ApplicationContext.h"
#include "sl/asset/AssetManager.hpp"
#include "sl/core/log/Logger.h"
#include "sl/core/path/PathManager.hpp"
#include "sl/geometry/Geometry.hpp"
#include "sl/geometry/Model.h"
#include "sl/gui/GUIProxy.h"
#include "sl/gui/Window.h"
#include "sl/misc/colors.hpp"
#include "sl/platform/shader/Shader.h"
#include "sl/platform/texture/Cubemap.h"
#include "sl/platform/texture/Texture.h"
#include "sl/platform/time/Clock.h"
#include "sl/rendering/camera/EulerCamera.h"
#include "sl/rendering/camera/FPSCamera.h"
#include "sl/rendering/camera/UserControllableCamera.h"
#include "sl/scene/Scene3D.h"
#include "sl/scene/Skybox.h"

#include "sl/core/async/AsyncEngine.hpp"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/RendererComponent.h"
#include "sl/scene/components/TransformComponent.h"

using namespace sl;
using namespace sl::scene;

class Context : public application::context::ApplicationContext {
public:
    void onInit() override {
        m_activeCamera = rendering::camera::EulerCamera::create(math::Vec3(0.0f), 1.0f, 8.0f);
        m_scene = scene::Scene3D::create();

        m_scene->setCamera(m_activeCamera);
        m_sceneManager->setActiveScene(m_scene);
	}

    void onAttach() override {
    }

    void onDetach() override {
    }

    void renderGUI(std::shared_ptr<gui::GUIProxy> guiProxy) override {
        static float width = 300;
        auto vp = m_viewport;

        // TODO: rework api, hierarchy is too deep and this shouldn't be based on RAII
        {
            auto rightWindow = guiProxy->createWindow("Starlight", { vp.width - width, 250 }, { width, 500 });
            m_activeCamera->onGUI(rightWindow);
            m_sceneManager->renderMainGUI(rightWindow);
            if (rightWindow.beginTreeNode("Assets")) {
                rightWindow.popTreeNode();
            }
            rightWindow.breakLine();
        }
        {
            auto leftWindow = guiProxy->createWindow("Scene", { 0, 250 }, { width, 350 });
            m_sceneManager->renderSceneGUI(leftWindow);
        }
    }

    void handleInput(std::shared_ptr<platform::input::Input> input) override {
        m_activeCamera->handleInput(input);
    }

    void update(float deltaTime, float time) override {
        m_activeCamera->update(deltaTime);
        m_sceneManager->update(deltaTime);
    }

    void render() override {
        m_sceneManager->render();
    }

private:
	int m_fakeDelay;

    std::shared_ptr<rendering::camera::UserControllableCamera> m_activeCamera;
    std::shared_ptr<scene::Scene3D> m_scene;
};
