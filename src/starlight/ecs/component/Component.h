#pragma once

namespace sl::gui {
class Window;
}

namespace sl::ecs::component {

enum class ComponentType : unsigned char {
    MODEL,
    TRANSFORM,
    PFX,
    MESH_GRID
};

class Component {
public:
    virtual ComponentType getType() const = 0;
    virtual void onGUI(gui::Window&) {}
};
}
