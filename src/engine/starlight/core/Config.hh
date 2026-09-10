#pragma once

#include "Core.hh"
#include "FileSystem.hh"
#include "Log.hh"

namespace sl {

struct Config {
    struct Log {
        log::Level level{log::Level::trace};
    };

    struct Paths {
        Path cwd{Path::cwd()};
        Path assets{Path::join(cwd, "assets")};
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
    Paths paths;
};

}  // namespace sl
