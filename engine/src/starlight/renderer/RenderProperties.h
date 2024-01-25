#pragma once

#include "starlight/core/Core.h"

#include "RenderMode.h"

namespace sl {

struct RenderProperties {
    RenderMode renderMode;
    u64 frameNumber;
};

}  // namespace sl
