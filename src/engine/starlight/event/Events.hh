#pragma once

#include "starlight/core/Core.hh"

namespace sl {

struct QuitRequestedEvent {
    Str reason{"No reason specified"};
};

}  // namespace sl
