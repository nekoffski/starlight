#pragma once

#include "starlight/math/Vector.hpp"

namespace sl::rendering::pfx {

struct ParticleGeneratorSettings {
    explicit ParticleGeneratorSettings(float radius = 0.7f, math::Vec3 minColor = math::VEC_ZEROS,
        math::Vec3 maxColor = math::VEC_ONES)
        : radius(radius)
        , minColor(minColor)
        , maxColor(maxColor) {
    }

    float radius;
    math::Vec3 minColor;
    math::Vec3 maxColor;
};
}