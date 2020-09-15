#pragma once

#include "DirectionalLightComponent.h"
#include "starlight/ecs/ComponentWrapper.h"
#include "starlight/gui/Utils.hpp"

namespace sl::scene::components {

class DirectionalLightComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGUI(gui::Window& window) override {
        DirectionalLightComponent& component = static_cast<DirectionalLightComponent&>(m_component);
        if (window.beginTreeNode("Directional light")) {
            window.displayText("Position");
            window.dragFloat3(gui::createHiddenLabel("dlcDirection"), component.direction, 0.01f, -1.0f, 1.0f);
            window.displayText("Color");
            window.colorPicker3(gui::createHiddenLabel("dlcColor"), component.color);

            window.popTreeNode();
        }
    }
};

struct DirectionalLightComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<DirectionalLightComponentWrapper>(component);
    }
};
}
