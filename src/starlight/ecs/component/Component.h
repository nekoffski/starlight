#pragma once

namespace starl::ecs::component {

enum class ComponentType : unsigned char {
    MODEL
};

class Component {
public:
    virtual ComponentType getType() const = 0;
};
}
