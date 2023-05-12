#pragma once

#include <string>

#include "nova/core/Id.h"
#include "Material.h"

namespace nova::gfx {

struct Geometry {
    core::Id id         = core::invalidId;
    core::Id internalId = core::invalidId;
    uint32_t generation = 0;
    std::string name;
    Material* material = nullptr;  // todo, use constructor instead
};

}  // namespace nova::gfx
