#pragma once

#include "Camera.h"

#include "sl/core/Logger.h"
#include "sl/gfx/ViewFrustum.h"
#include "sl/gfx/camera/Camera.h"
#include "sl/gui/GuiApi.h"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"

namespace sl::gfx::camera {

class FPSCamera : public Camera {
public:
    explicit FPSCamera(const ViewFrustum& viewFrustum)
        : Camera(viewFrustum) {
        m_position = math::Vec3 { 0.0f, 5.0f, 0.0f };
    }

    void update(float deltaTime, core::Input& input) override {
        float velocity = deltaTime * m_speed;

        if (m_direction & directionUp)
            m_position += m_front * velocity;

        if (m_direction & directionRight)
            m_position += m_right * velocity;

        if (m_direction & directionDown)
            m_position -= m_front * velocity;

        if (m_direction & directionLeft)
            m_position -= m_right * velocity;

        if (m_x == 0.0f && m_y == 0.0f)
            return;

        float xoffset = m_x - m_prevX;
        float yoffset = m_prevY - m_y;

        m_prevX = m_x;
        m_prevY = m_y;

        xoffset /= (360.0f * 4.0f);
        yoffset /= (360.0f * 4.0f);

        m_yaw += xoffset;
        m_pitch = math::circularRange(m_pitch - yoffset, -3.13f, 3.13f);

        m_front = math::normalize(-math::Vec3 {
            std::cos(m_yaw) * std::cos(m_pitch),
            std::sin(m_pitch),
            std::sin(m_yaw) * std::cos(m_pitch) });

        m_right = math::cross(m_front, worldUp);
        m_up = math::cross(m_right, m_front);
    }

    // void handleInput(core::Input& input) override {
    //     m_direction = directionNone;

    //     if (input.isKeyPressed(STARL_KEY_UP))
    //         m_direction |= directionUp;

    //     if (input.isKeyPressed(STARL_KEY_RIGHT))
    //         m_direction |= directionRight;

    //     if (input.isKeyPressed(STARL_KEY_DOWN))
    //         m_direction |= directionDown;

    //     if (input.isKeyPressed(STARL_KEY_LEFT))
    //         m_direction |= directionLeft;

    //     auto [mouseX, mouseY] = input.getMousePosition();
    //     m_x = mouseX;
    //     m_y = mouseY;
    // }

    virtual const math::Mat4 getViewMatrix() {
        return math::lookAt(m_position, m_position + m_front, m_up);
    }

    void onGui(gui::GuiApi&) override {
    }

private:
    float m_x { 0.0f };
    float m_y { 0.0f };

    float m_pitch { 0.0f };
    float m_yaw { 0.0f };

    float m_prevX;
    float m_prevY;

    float m_speed = 5.0f;

    Direction m_direction;
};
}
