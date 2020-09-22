#pragma once

#include "sl/math/Vector.hpp"

namespace sl::rendering::pfx {

struct ParticleGeneratorSettings {
    float radius = 0.7f;
    float minScale = 0.1f;
    float maxScale = 0.2f;
    float deltaScale = 0.1f;
    float minSpeed = 0.5f;
    float maxSpeed = 1.7f;
    float directionFactor = 1.0f;
    math::Vec3 minColor = math::VEC_ZEROS;
    math::Vec3 maxColor = math::VEC_ONES;
};
}