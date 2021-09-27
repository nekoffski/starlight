#pragma once

#include <memory>
#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "sl/cfg/Config.h"
#include "sl/core/Macros.h"

#include "Flags.h"
#include "Geometry.h"
#include "RendererInfo.h"
#include "Shaders.h"
#include "Visual.h"
#include "World.h"

namespace sl::glob {

struct Globals : kc::core::Singleton<Globals> {
    void init();

    World world;
    Flags flags;
    Visual visual;
    RendererInfo rendererInfo;

    cfg::Config config;

    std::unique_ptr<Shaders> shaders;
    std::unique_ptr<Geometry> geom;
};

}
