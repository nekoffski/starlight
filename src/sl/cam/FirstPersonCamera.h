#pragma once

#include <kc/core/Log.h>

#include "sl/cam/Camera.h"
#include "sl/core/InputManager.h"
#include "sl/gfx/ViewFrustum.h"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"

namespace sl::cam {

class FirstPersonCamera : public Camera {
   public:
    explicit FirstPersonCamera(const gfx::ViewFrustum& viewFrustum = gfx::ViewFrustum{0, 0})
        : Camera(viewFrustum) {
        m_position = math::Vec3{0.0f, 0.0f, 0.0f};

        LOG_TRACE("Creating FirstPersonCamera");
    }

    void update(float deltaTime) override {
        float velocity = deltaTime * m_speed;

        auto& inputManager = core::InputManager::get();

        auto pos = inputManager.getMousePosition();
        m_x      = pos.x;
        m_y      = pos.y;

        if (inputManager.isKeyPressed(STARL_KEY_W)) m_position += m_front * velocity;

        if (inputManager.isKeyPressed(STARL_KEY_S)) m_position -= m_front * velocity;

        if (inputManager.isKeyPressed(STARL_KEY_A)) m_position -= m_right * velocity;

        if (inputManager.isKeyPressed(STARL_KEY_D)) m_position += m_right * velocity;

        if (m_x == 0.0f && m_y == 0.0f) return;

        float xoffset = m_x - m_prevX;
        float yoffset = m_prevY - m_y;

        m_prevX = m_x;
        m_prevY = m_y;

        xoffset /= (360.0f * 4.0f);
        yoffset /= (360.0f * 4.0f);

        m_yaw += xoffset;
        m_pitch = math::circularRange(m_pitch + yoffset, -3.13f, 3.13f);

        m_front = math::normalize(-math::Vec3{
            std::cos(m_yaw) * std::cos(m_pitch), std::sin(m_pitch),
            std::sin(m_yaw) * std::cos(m_pitch)});

        m_right = math::normalize(math::cross(m_front, math::worldUp));
        m_up    = math::normalize(math::cross(m_right, m_front));
    }

    virtual const math::Mat4 getViewMatrix() const override {
        return math::lookAt(m_position, m_position + m_front, math::worldUp);
    }

    void onGui() override {}

   private:
    float m_x{0.0f};
    float m_y{0.0f};

    float m_pitch{0.0f};
    float m_yaw{0.0f};

    float m_prevX;
    float m_prevY;

    float m_speed = 5.0f;
};
}  // namespace sl::cam
