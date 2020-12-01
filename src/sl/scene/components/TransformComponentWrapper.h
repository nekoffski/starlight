#pragma once

#include "TransformComponent.h"
#include "sl/ecs/ComponentWrapper.h"
#include "sl/gui/Utils.hpp"

namespace sl::scene::components {

class TransformComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGUI(gui::Panel& panel) override {
        TransformComponent& component = static_cast<TransformComponent&>(m_component);
        if (panel.beginTreeNode("Transform")) {
            panel.displayText("Translation");
            panel.dragFloat3(gui::createHiddenLabel("translation"), component.position, 0.1f);
            panel.displayText("Rotation");
            panel.dragFloat3(gui::createHiddenLabel("rotation"), component.rotation, 0.5f, 0.0f, 360.0f);
            panel.displayText("Scale");
			panel.dragFloat3(gui::createHiddenLabel("scale"), component.scale, 0.5f, 0.0f, 360.0f);
			panel.popTreeNode();
        }
    }
};

struct TransformComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<TransformComponentWrapper>(component);
    }
};
}
