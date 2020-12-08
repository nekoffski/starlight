#pragma once

#include "TransformComponent.h"
#include "sl/ecs/ComponentWrapper.h"
#include "sl/gui/Utils.hpp"

namespace sl::scene::components {

class TransformComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGui(gui::GuiProxy& gui) override {
        TransformComponent& component = static_cast<TransformComponent&>(m_component);
        if (gui->beginTreeNode("Transform")) {
            gui->displayText("Translation");
            gui->dragFloat3(gui::createHiddenLabel("translation"), component.position, 0.1f);
            gui->displayText("Rotation");
            gui->dragFloat3(gui::createHiddenLabel("rotation"), component.rotation, 0.5f, 0.0f, 360.0f);
            gui->displayText("Scale");
            gui->dragFloat3(gui::createHiddenLabel("scale"), component.scale, 0.5f, 0.0f, 360.0f);
            gui->popTreeNode();
        }
    }
};

struct TransformComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<TransformComponentWrapper>(component);
    }
};
}
