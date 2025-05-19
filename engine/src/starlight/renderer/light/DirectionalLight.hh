#pragma once

#include "starlight/math/Core.hh"

namespace sl {

struct alignas(16) DirectionalLight {
    Vec4<f32> color     = { 0.8f, 0.8f, 0.8f, 1.0f };
    Vec3<f32> direction = { -0.57735f, -0.57735f, -0.57735f };
};

std::string toString(const DirectionalLight& l);

}  // namespace sl