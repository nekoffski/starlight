#pragma once

#include "starlight/core/Core.h"

#include "starlight/renderer/Model.h"

namespace sl {

struct ModelComponent {
    u64 id;
    u64 entityId;

    Model* model;
};

}  // namespace sl