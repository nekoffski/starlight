#pragma once

#include "SceneManager.h"

#include <exception>

#include "sl/asset/AssetManager.hpp"
#include "sl/ecs/Entity.h"
#include "sl/geometry/Geometry.hpp"
#include "sl/gui/Window.h"
#include "sl/math/Vector.hpp"
#include "sl/misc/colors.hpp"
#include "sl/platform/shader/Shader.h"
#include "sl/rendering/Renderer.h"
#include "sl/scene/Scene2D.h"
#include <memory>


namespace sl::scene {

class SceneManager2D : public SceneManager {

public:
    explicit SceneManager2D(std::shared_ptr<rendering::Renderer> renderer)
        : m_renderer(renderer) {
    }

    void update(float) {}

    void setActiveScene(std::shared_ptr<Scene> scene) override {
    }

    void render() override {
    }

    void renderSceneGUI(gui::Window& window) {
    }
    void renderMainGUI(gui::Window& window) {
    }

private:
    std::shared_ptr<platform::shader::Shader> m_shader;
    std::shared_ptr<rendering::Renderer> m_renderer;
    std::shared_ptr<Scene2D> m_scene;
    std::weak_ptr<ecs::Entity> m_activeEntity;
};
}
