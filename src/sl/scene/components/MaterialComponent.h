#pragma once

#include "sl/ecs/Component.h"

#include "sl/core/misc/colors.hpp"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

struct MaterialComponent : ecs::Component {
    explicit MaterialComponent(math::Vec3 ambientColor = core::misc::COL_WHITE, math::Vec3 diffuseColor = core::misc::COL_WHITE,
        math::Vec3 specularColor = core::misc::COL_WHITE, float shininess = 32.0f)
        : ambientColor(ambientColor)
        , diffuseColor(diffuseColor)
        , specularColor(specularColor)
        , shininess(shininess) {
    }

    void onGui(gui::GuiApi& gui) override {
        if (gui.beginTreeNode("Material")) {
            gui.displayText("Ambient color");
            gui.colorPicker3(gui::createHiddenLabel("Ambient color"), ambientColor);
            gui.displayText("Diffuse color");
            gui.colorPicker3(gui::createHiddenLabel("Diffuse color"), diffuseColor);
            gui.displayText("Specular color");
            gui.colorPicker3(gui::createHiddenLabel("Specular color"), specularColor);
            gui.displayText("Shininess");
            gui.dragFloat(gui::createHiddenLabel("rotation"), shininess, 0.5f, 0.0f, 128.0f);
            gui.popTreeNode();
        }
    }

    math::Vec3 ambientColor;
    math::Vec3 diffuseColor;
    math::Vec3 specularColor;

    float shininess;
};
}
