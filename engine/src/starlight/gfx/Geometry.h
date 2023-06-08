#pragma once

#include <string>

#include "starlight/core/Id.h"
#include "Material.h"

namespace sl::gfx {

struct Geometry {
    core::Id id         = core::invalidId;
    core::Id internalId = core::invalidId;
    uint32_t generation = 0;
    std::string name;
    Material* material = nullptr;  // todo, use constructor instead
};

}  // namespace sl::gfx
