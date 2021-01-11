#pragma once

#include <memory>

#include "sl/core/Macros.h"

#include "globals/Geometry.h"
#include "globals/Shaders.h"

namespace sl::utils {

struct Globals {
    SL_STATIC_CLASS(Globals);

    static void init() {
        shaders = std::make_unique<globals::Shaders>();
        geometry = std::make_unique<globals::Geometry>();
    }

    inline static std::unique_ptr<globals::Shaders> shaders;
    inline static std::unique_ptr<globals::Geometry> geometry;
};
}
