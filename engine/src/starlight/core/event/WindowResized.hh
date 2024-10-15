#pragma once

#include <kc/core/Log.h>
#include <kc/core/Macros.h>
#include <kc/core/String.h>

#include "starlight/core/Core.hh"

namespace sl {

struct WindowResized {
    Vec2<u32> size;
};

}  // namespace sl

DEFINE_TO_STRING(sl::WindowResized, event) {
    return fmt::format("WindowResized[{}/{}]", event.size.w, event.size.h);
}
