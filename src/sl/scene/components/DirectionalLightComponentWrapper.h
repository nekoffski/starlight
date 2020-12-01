#pragma once

#include "DirectionalLightComponent.h"
#include "sl/ecs/ComponentWrapper.h"
#include "sl/gui/Utils.hpp"
#include "sl/math/Utils.hpp"

namespace sl::scene::components {

class DirectionalLightComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGUI(gui::Panel& panel) override {
        DirectionalLightComponent& component = static_cast<DirectionalLightComponent&>(m_component);
        if (panel.beginTreeNode("Directional light")) {
            panel.displayText("Position");

            if (panel.dragFloat3(gui::createHiddenLabel("dlcDirection"), component.direction, 0.01f, -1.0f, 1.0f)) {
                component.viewMatrix = math::lookAt(component.direction, math::Vec3{ 0.0f }, math::Vec3{ 0.0f, 1.0f, 0.0f });
                component.spaceMatrix = LIGHT_PROJECTION * component.viewMatrix;
            }

            panel.displayText("Color");
            panel.colorPicker3(gui::createHiddenLabel("dlcColor"), component.color);

            panel.popTreeNode();
        }
    }
};

struct DirectionalLightComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<DirectionalLightComponentWrapper>(component);
    }
};
}
