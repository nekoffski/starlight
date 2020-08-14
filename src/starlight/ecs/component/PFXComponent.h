#pragma once

#include "Component.h"

#include <memory>

#include "starlight/gui/Utils.hpp"
#include "starlight/gui/Window.h"

namespace sl::ecs::component {

class PFXComponent : public Component {
public:
    PFXComponent() {
    }

    ComponentType getType() const {
        return ComponentType::PFX;
    }

    void onGUI(gui::Window& window) override {
        if (window.beginTreeNode("Particle effect"))
            window.popTreeNode();
    }

private:
};
}