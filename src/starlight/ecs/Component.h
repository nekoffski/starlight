#pragma once

#include "Entity.h"

namespace sl::ecs {

struct Component {
    std::shared_ptr<Entity> entity;
};
}