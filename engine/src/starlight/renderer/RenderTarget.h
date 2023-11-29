#pragma once

#include <vector>

#include "starlight/core/Core.h"

#include "Texture.h"

namespace sl {

struct RenderTarget {
    enum class SyncMode { noSync, syncWithWindowSize };

    SyncMode syncMode;
    std::vector<Texture*> attachments;
};

}  // namespace sl