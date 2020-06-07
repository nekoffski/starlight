#pragma once

namespace starl::gui {
class Window;
}

namespace starl::ecs::component {

enum class ComponentType : unsigned char {
    MODEL,
    TRANSFORM,
    PFX
};

class Component {
public:
    virtual ComponentType getType() const = 0;
    virtual void onGUI(gui::Window&) {}
};
}
