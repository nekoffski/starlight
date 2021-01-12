#pragma once

#include "sl/ecs/Component.h"

#include "sl/core/misc/colors.hpp"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

struct PointLightComponent : public ecs::Component {
    explicit PointLightComponent(math::Vec3 position = math::Vec3{ 0.0f, 0.0f, 0.0f }, math::Vec3 color = core::misc::COL_WHITE,
        float attenuationA = 0.05f, float attenuationB = 0.1f, float attenuationC = 0.3f)
        : position(position)
        , color(color)
        , attenuationA(attenuationA)
        , attenuationB(attenuationB)
        , attenuationC(attenuationC) {
    }

    void onGui(gui::GuiApi& gui) override {
        if (gui.beginTreeNode("Point light")) {
            gui.displayText("Position");
            gui.dragFloat3(gui::createHiddenLabel("plcPositon"), position, 0.1f);
            gui.displayText("Color");
            gui.colorPicker3(gui::createHiddenLabel("plcColor"), color);
            gui.displayText("Attenuation A");
            gui.dragFloat(gui::createHiddenLabel("plcAttenuationA"), attenuationA, 0.001f, 0.0f, 10.0f);
            gui.displayText("Attenuation B");
            gui.dragFloat(gui::createHiddenLabel("plcAttenuationB"), attenuationB, 0.001f, 0.0f, 10.0f);
            gui.displayText("Attenuation C");
            gui.dragFloat(gui::createHiddenLabel("plcAttenuationC"), attenuationC, 0.001f, 0.0f, 10.0f);
        }
    }

    math::Vec3 position;
    math::Vec3 color;
    float attenuationA;
    float attenuationB;
    float attenuationC;
};
}
