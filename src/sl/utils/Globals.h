#pragma once

#include <memory>
#include <unordered_map>

#include "sl/core/Macros.h"

#include "globals/Config.h"
#include "globals/Flags.h"
#include "globals/Geometry.h"
#include "globals/Shaders.h"
#include "globals/World.h"

namespace sl::utils {

struct Globals {
    SL_SINGLETON(Globals);

    void init();

    globals::World world;
    globals::Config config;
    globals::Flags flags;

    std::unique_ptr<globals::Shaders> shaders;
    std::unique_ptr<globals::Geometry> geom;
};

}

#define GLOBALS() sl::utils::Globals::instance()
