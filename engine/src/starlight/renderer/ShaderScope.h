#pragma once

#include "starlight/core/Core.h"

namespace sl {

enum class ShaderScope : u8 {
    global,    // updated once per frame
    instance,  // per instance
    local      // per object
};

}
