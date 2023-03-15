#pragma once

#include <string>

#include "nova/core/Id.h"
#include "Material.h"

namespace nova::gfx {

struct Geometry {
    core::Id id;
    core::Id internalId;
    uint32_t generation;
    std::string name;
    Material* material;
};

}  // namespace nova::gfx
