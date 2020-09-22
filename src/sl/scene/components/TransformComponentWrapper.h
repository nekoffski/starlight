#pragma once

#include "TransformComponent.h"
#include "sl/ecs/ComponentWrapper.h"
#include "sl/gui/Utils.hpp"

namespace sl::scene::components {

class TransformComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGUI(gui::Window& window) override {
        TransformComponent& component = static_cast<TransformComponent&>(m_component);
        if (window.beginTreeNode("Transform")) {
            window.displayText("Translation");
            window.dragFloat3(gui::createHiddenLabel("translation"), component.position, 0.1f);
            window.displayText("Rotation");
            window.dragFloat3(gui::createHiddenLabel("rotation"), component.rotation, 0.5f, 0.0f, 360.0f);
            window.popTreeNode();
        }
    }
};

struct TransformComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<TransformComponentWrapper>(component);
    }
};
}
