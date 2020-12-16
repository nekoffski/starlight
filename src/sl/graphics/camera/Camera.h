#pragma once

#include <cstdint>

#include "sl/core/Logger.h"
#include "sl/gui/GuiApi.h"
#include "sl/gui/fwd.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

namespace sl::graphics::camera {

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
    explicit Camera(math::Vec3 position = math::Vec3{ 0.0f }, math::Vec3 up = WORLD_UP,
        math::Vec3 front = WORLD_FRONT, math::Vec3 right = WORLD_RIGHT)
        : m_position(std::move(position))
        , m_up(std::move(up))
        , m_front(std::move(front))
        , m_right(std::move(right)) {
    }

    virtual void update(float) = 0;

    virtual void onGui(gui::GuiApi&) {}

    const math::Vec3& getPosition() {
        return m_position;
    }

    const math::Vec3& getUp() {
        return m_up;
    }

    const math::Vec3& getFront() {
        return m_front;
    }

    const math::Vec3& getRight() {
        return m_right;
    }

    virtual const math::Mat4 getViewMatrix() {
        return math::lookAt(m_position, m_front, m_up);
    }

protected:
    math::Vec3 m_position;
    math::Vec3 m_up;
    math::Vec3 m_front;
    math::Vec3 m_right;
};
}
