#pragma once

#include <string>

#include "Material.h"
#include "starlight/core/utils/Id.h"

namespace sl {

struct Geometry {
    Id id = invalidId;
    Id internalId = invalidId;
    uint32_t generation = 0;
    std::string name;
    Material* material = nullptr;  // todo, use constructor instead
};

}  // namespace sl
