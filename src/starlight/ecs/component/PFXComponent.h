#pragma once

#include <memory>

#include <starlight/ecs/component/Component.h>
#include <starlight/gui/Utils.hpp>
#include <starlight/gui/Window.h>

namespace starl::ecs::component {

class PFXComponent : public Component {
public:
    PFXComponent() {
    }

    ComponentType getType() const {
        return ComponentType::PFX;
    }

    void onGUI(gui::Window& window) override {
        if (window.beginTreeNode("Particle effect")) {
            window.popTreeNode();
        }
    }

private:
};
}