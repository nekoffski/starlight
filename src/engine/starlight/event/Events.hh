#pragma once

#include "starlight/core/Core.hh"

namespace sl {

struct QuitRequestedEvent {
    Str reason{"No reason specified"};
};

struct WindowResizedEvent {
    u32 width;
    u32 height;
};

}  // namespace sl
