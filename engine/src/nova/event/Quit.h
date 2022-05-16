#pragma once

#include <string>

namespace nova::event {

struct QuitEvent {
    std::string reason;
};

}  // namespace nova::event
