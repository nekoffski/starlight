#pragma once

#include <memory>

#include <starlight/ecs/component/Component.h>

namespace starl::ecs::system {

class System {
public:
    virtual void processComponent(std::shared_ptr<component::Component>&) = 0;
};
}