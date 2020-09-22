#pragma once

#include "PointLightComponent.h"
#include "sl/ecs/ComponentWrapper.h"
#include "sl/gui/Utils.hpp"

namespace sl::scene::components {

class PointLightComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGUI(gui::Window& window) override {
        PointLightComponent& component = static_cast<PointLightComponent&>(m_component);
        if (window.beginTreeNode("Point light")) {
            window.displayText("Position");
            window.dragFloat3(gui::createHiddenLabel("plcPositon"), component.position, 0.1f);
            window.displayText("Color");
            window.colorPicker3(gui::createHiddenLabel("plcColor"), component.color);
            window.displayText("Attenuation A");
            window.dragFloat(gui::createHiddenLabel("plcAttenuationA"), component.attenuationA, 0.001f, 0.0f, 10.0f);
            window.displayText("Attenuation B");
            window.dragFloat(gui::createHiddenLabel("plcAttenuationB"), component.attenuationB, 0.001f, 0.0f, 10.0f);
            window.displayText("Attenuation C");
            window.dragFloat(gui::createHiddenLabel("plcAttenuationC"), component.attenuationC, 0.001f, 0.0f, 10.0f);
        }
    }
};

struct PointLightComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<PointLightComponentWrapper>(component);
    }
};
}
