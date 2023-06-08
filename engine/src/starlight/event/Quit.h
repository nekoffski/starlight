#pragma once

#include <string>

namespace sl::event {

struct QuitEvent {
    std::string reason;
};

}  // namespace sl::event
