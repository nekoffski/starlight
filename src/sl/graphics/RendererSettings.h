#pragma once

#include "sl/platform/gpu/OpenGlDefines.h"

namespace sl::graphics {

struct RendererSettings {
    bool enableDepthMask = true;
    unsigned int polygonMode = STARL_FILL;
};
}
