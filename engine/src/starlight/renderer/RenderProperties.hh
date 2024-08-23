#pragma once

#include "starlight/core/Core.hh"

#include "RenderMode.hh"

namespace sl {

struct RenderProperties {
    RenderMode renderMode;
    u64 frameNumber;
};

}  // namespace sl
