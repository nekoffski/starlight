#pragma once

#include "sl/ecs/Component.h"

#include <memory>

#include "sl/gui/Utils.hpp"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

struct TransformComponent : public ecs::Component {
    TransformComponent(math::Vec3 position = math::Vec3{ 0.0f },
        math::Vec3 rotation = math::Vec3{ 0.0f }, math::Vec3 scale = math::Vec3{ 1.0f })
        : position(position)
        , rotation(rotation)
        , scale(scale) {
    }

    void onGui(gui::GuiApi& gui) override {
        if (gui.beginTreeNode("Transform")) {
            gui.displayText("Translation");
            gui.dragFloat3(gui::createHiddenLabel("translation"), position, 0.1f);
            gui.displayText("Rotation");
            gui.dragFloat3(gui::createHiddenLabel("rotation"), rotation, 0.5f, 0.0f, 360.0f);
            gui.displayText("Scale");
            gui.dragFloat3(gui::createHiddenLabel("scale"), scale, 0.5f, 0.0f, 360.0f);
            gui.popTreeNode();
        }
    }

    math::Vec3 position;
    math::Vec3 rotation;
    math::Vec3 scale;

    math::Mat4 operator()() {
        return math::scale(scale) * math::translate(position) * math::createRotationMatrix(math::toRadians(rotation));
    }
};
}
