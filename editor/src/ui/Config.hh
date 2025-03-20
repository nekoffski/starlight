#pragma once

#include <starlight/core/math/Core.hh>

namespace sle {

struct Config {
    static Config createDefault();

    sl::Vec2<sl::f32> layoutSizeRatio;
};

}  // namespace sle
