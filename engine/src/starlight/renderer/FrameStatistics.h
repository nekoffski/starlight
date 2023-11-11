#pragma once

#include <starlight/core/Core.h>

namespace sl {

struct FrameStatistics {
    u64 renderedVertices = 0;
    u64 frameNumber      = 0;
};

}  // namespace sl