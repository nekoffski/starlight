#pragma once

#include <kc/core/Log.h>
#include <kc/core/Macros.h>

namespace nova::event {

struct WindowResized {
    uint32_t width;
    uint32_t height;
};

inline std::string toString(const WindowResized& event) {
    return fmt::format("WindowResized[{}/{}]", event.width, event.height);
}

TO_STRING_STREAM(WindowResized);

}  // namespace nova::event