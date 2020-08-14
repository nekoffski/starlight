#pragma once

#include <memory>

#include "starlight/ecs/component/Component.h"
#include "starlight/ecs/entity/Entity.h"

namespace sl::ecs::system {

class System {
public:
    virtual void processComponent(std::shared_ptr<component::Component>&, std::shared_ptr<entity::Entity>&) = 0;
};
}