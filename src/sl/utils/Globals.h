#pragma once

#include <memory>
#include <unordered_map>

#include "sl/core/Macros.h"

#include "Config.h"
#include "Flags.h"
#include "Geometry.h"
#include "Info.h"
#include "Shaders.h"
#include "Visual.h"
#include "World.h"

namespace sl::utils {

struct Globals {
    LOG_SINGLETON(Globals);

    void init();

    World world;
    Config config;
    Flags flags;
    Visual visual;
    Info info;

    std::unique_ptr<Shaders> shaders;
    std::unique_ptr<Geometry> geom;
};

}

#define GLOBALS() sl::utils::Globals::instance()
