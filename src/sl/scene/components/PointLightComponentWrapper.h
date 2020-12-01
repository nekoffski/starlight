#pragma once

#include "PointLightComponent.h"
#include "sl/ecs/ComponentWrapper.h"
#include "sl/gui/Utils.hpp"

namespace sl::scene::components {

class PointLightComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGUI(gui::Panel& panel) override {
        PointLightComponent& component = static_cast<PointLightComponent&>(m_component);
        if (panel.beginTreeNode("Point light")) {
            panel.displayText("Position");
            panel.dragFloat3(gui::createHiddenLabel("plcPositon"), component.position, 0.1f);
            panel.displayText("Color");
            panel.colorPicker3(gui::createHiddenLabel("plcColor"), component.color);
            panel.displayText("Attenuation A");
            panel.dragFloat(gui::createHiddenLabel("plcAttenuationA"), component.attenuationA, 0.001f, 0.0f, 10.0f);
            panel.displayText("Attenuation B");
            panel.dragFloat(gui::createHiddenLabel("plcAttenuationB"), component.attenuationB, 0.001f, 0.0f, 10.0f);
            panel.displayText("Attenuation C");
            panel.dragFloat(gui::createHiddenLabel("plcAttenuationC"), component.attenuationC, 0.001f, 0.0f, 10.0f);
        }
    }
};

struct PointLightComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<PointLightComponentWrapper>(component);
    }
};
}
