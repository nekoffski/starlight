#pragma once

#include "PointLightComponent.h"
#include "sl/ecs/ComponentWrapper.h"
#include "sl/gui/Utils.hpp"

namespace sl::scene::components {

class PointLightComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGui(gui::GuiProxy& gui) override {
        PointLightComponent& component = static_cast<PointLightComponent&>(m_component);
        if (gui->beginTreeNode("Point light")) {
            gui->displayText("Position");
            gui->dragFloat3(gui::createHiddenLabel("plcPositon"), component.position, 0.1f);
            gui->displayText("Color");
            gui->colorPicker3(gui::createHiddenLabel("plcColor"), component.color);
            gui->displayText("Attenuation A");
            gui->dragFloat(gui::createHiddenLabel("plcAttenuationA"), component.attenuationA, 0.001f, 0.0f, 10.0f);
            gui->displayText("Attenuation B");
            gui->dragFloat(gui::createHiddenLabel("plcAttenuationB"), component.attenuationB, 0.001f, 0.0f, 10.0f);
            gui->displayText("Attenuation C");
            gui->dragFloat(gui::createHiddenLabel("plcAttenuationC"), component.attenuationC, 0.001f, 0.0f, 10.0f);
        }
    }
};

struct PointLightComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<PointLightComponentWrapper>(component);
    }
};
}
