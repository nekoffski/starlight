#pragma once

#include <kc/core/Log.h>
#include <kc/core/Macros.h>

#include "starlight/core/Core.hh"

namespace sl {

struct WindowResized {
    Vec2<u32> size;
};

inline std::string toString(const WindowResized& event) {
    return fmt::format("WindowResized[{}/{}]", event.size.w, event.size.h);
}

TO_STRING_STREAM(WindowResized);

}  // namespace sl