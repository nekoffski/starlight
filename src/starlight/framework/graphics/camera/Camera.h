#pragma once

#include <cstdint>

#include <starlight/math/Matrix.hpp>
#include <starlight/math/Vector.hpp>

namespace starl::gui {
class Window;
}

namespace starl::framework::graphics::camera {

using Direction = u_int8_t;

// clang-format off
constexpr Direction DIR_NONE   = 0b0000;
constexpr Direction DIR_UP     = 0b0001;
constexpr Direction DIR_RIGHT  = 0b0010;
constexpr Direction DIR_DOWN   = 0b0100;
constexpr Direction DIR_LEFT   = 0b1000;

const math::Vec3 WORLD_UP    = { 0.0f, 1.0f, 0.0f };
const math::Vec3 WORLD_FRONT = { 0.0f, 0.0f, 1.0f };
const math::Vec3 WORLD_RIGHT = { 1.0f, 0.0f, 0.0f };
// clang-format on

class Camera {
public:
    virtual void update(float) = 0;

    virtual const math::Vec3& getPosition() = 0;
    virtual const math::Vec3& getUp() = 0;
    virtual const math::Vec3& getFront() = 0;
    virtual const math::Vec3& getRight() = 0;

    virtual const math::Mat4 getViewMatrix() = 0;

    virtual void onGUI(gui::Window&) {}
};
}