#pragma once

#include "Core.hh"
#include "Log.hh"

namespace sl {

struct Config {
    struct Log {
        log::Level level{log::Level::trace};
    };

    struct Renderer {};

    Log log;
    Renderer renderer;
};

}  // namespace sl
