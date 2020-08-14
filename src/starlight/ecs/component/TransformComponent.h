#pragma once

#include "starlight/ecs/component/Component.h"

#include <memory>

#include "starlight/gui/Utils.hpp"
#include "starlight/gui/Window.h"
#include "starlight/math/Vector.hpp"

namespace sl::ecs::component {

class TransformComponent : public Component {
public:
    TransformComponent()
        : m_translation(0.0f)
        , m_rotation(0.0f) {
    }

    math::Vec3 getTranslation() const {
        return m_translation;
    }

    math::Vec3 getRotation() const {
        return m_rotation;
    }

    ComponentType getType() const override {
        return ComponentType::TRANSFORM;
    }

    void onGUI(gui::Window& window) override {
        if (window.beginTreeNode("Transform")) {
            window.displayText("Translation");
            window.dragFloat3(gui::createHiddenLabel("translation"), m_translation, 0.1f);
            window.displayText("Rotation");
            window.dragFloat3(gui::createHiddenLabel("rotation"), m_rotation, 0.5f, 0.0f, 360.0f);
            window.popTreeNode();
        }
    }

private:
    math::Vec3 m_translation;
    math::Vec3 m_rotation;
};
}