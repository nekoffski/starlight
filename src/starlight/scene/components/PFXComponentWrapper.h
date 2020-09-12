#pragma once

#include "PFXComponent.h"
#include "starlight/ecs/ComponentWrapper.h"
#include "starlight/gui/Utils.hpp"

namespace sl::scene::components {

class PFXComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGUI(gui::Window& window) override {
        PFXComponent& component = static_cast<PFXComponent&>(m_component);
        if (window.beginTreeNode("PFX")) {
            window.displayText("Position");
            window.dragFloat3(gui::createHiddenLabel("pfxPositon"), component.position, 0.1f);
        }
    }
};

struct PFXComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<PFXComponentWrapper>(component);
    }
};
}
