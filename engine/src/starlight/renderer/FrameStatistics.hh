#pragma once

#include "starlight/core/Core.hh"

namespace sl {

struct FrameStatistics {
    u64 renderedVertices = 0;
    u64 frameNumber      = 0;
    float deltaTime      = 0;
};

}  // namespace sl