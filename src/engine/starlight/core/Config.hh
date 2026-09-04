#pragma once

#include "Core.hh"
#include "Log.hh"

namespace sl {

struct Config {
    struct Log {
        log::Level level{log::Level::trace};
    };

    struct Renderer {
        u16 maxFramesInFlight{3u};
        u16 maxTextures{64u};
        u16 maxMeshes{128u};
        u16 maxRenderPasses{8u};
        u16 maxShaders{32u};
    };

    Log log;
    Renderer renderer;
};

}  // namespace sl
