#pragma once

#include "starlight/core/Core.h"
#include "starlight/renderer/Material.h"

namespace sl {

struct MaterialComponent {
    u64 id;
    u64 entityId;

    Material* material = nullptr;
};

}  // namespace sl