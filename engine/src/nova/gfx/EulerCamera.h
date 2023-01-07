#pragma once

#include "Camera.h"

#include "nova/math/Glm.h"
#include "nova/core/WindowManager.h"

#include "nova/platform/Keys.h"

namespace nova::gfx {

class EulerCamera : public Camera {
   public:
    struct Properties {
        math::Vec3f target;
        float radius;
    };

    explicit EulerCamera(const Properties& props)
        : m_target(props.target), m_radius(props.radius), m_pitch(90.0f), m_yaw(90.0f) {
        recalculateVectors();
    }

    math::Mat4f getViewMatrix() const override { return m_viewMatrix; }

    math::Mat4f getProjectionMatrix() const override {
        return glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 1000.0f);
    }

    math::Vec3f getPosition() const override { return m_position; }

    void update(float deltaTime) override {
        static constexpr float speed = 100.0f;

        processInput(speed * deltaTime);
        truncateCoefficients();
        recalculateVectors();
        updateViewMatrix();
    }

    void processInput(const float speed) {
        if (core::WindowManager::get().isKeyPressed(NOVA_KEY_W)) m_pitch -= speed;
        if (core::WindowManager::get().isKeyPressed(NOVA_KEY_S)) m_pitch += speed;
        if (core::WindowManager::get().isKeyPressed(NOVA_KEY_D)) m_yaw -= speed;
        if (core::WindowManager::get().isKeyPressed(NOVA_KEY_A)) m_yaw += speed;

        if (core::WindowManager::get().isKeyPressed(NOVA_KEY_1)) m_radius += speed / 10.0f;
        if (core::WindowManager::get().isKeyPressed(NOVA_KEY_2)) m_radius -= speed / 10.0f;
    }

    void truncateCoefficients() {
        if (m_yaw >= 360.0f) m_yaw = 0.0f;
        if (m_yaw < 0.0f) m_yaw = 360.0f;

        if (m_pitch > 170.0f) m_pitch = 170.0f;
        if (m_pitch < 10.0f) m_pitch = 10.0f;

        static constexpr float maxRadius = 25.0f;

        if (m_radius < 0.0f) m_radius = 0.0f;
        if (m_radius > maxRadius) m_radius = maxRadius;
    }

    void recalculateVectors() {
        const static math::Vec3f worldUp = math::Vec3f{0.0f, 1.0f, 0.0f};

        const auto pitchRadians = glm::radians(m_pitch);
        const auto yawRadians   = glm::radians(m_yaw);

        // clang-format off
        m_position = m_radius * math::Vec3f {
            std::sin(pitchRadians) * std::cos(yawRadians),
            std::cos(pitchRadians),
            std::sin(pitchRadians) * std::sin(yawRadians)
        };
        // clang-format on

        m_front = glm::normalize(m_target - m_position);
        m_right = glm::cross(m_front, worldUp);
        m_up    = glm::cross(m_right, m_front);
    }

    void updateViewMatrix() { m_viewMatrix = glm::lookAt(m_position, m_target, m_up); }

   private:
    math::Vec3f m_target;
    float m_radius;

    math::Vec3f m_position;

    math::Vec3f m_front;
    math::Vec3f m_right;
    math::Vec3f m_up;

    float m_yaw;
    float m_pitch;

    math::Mat4f m_viewMatrix;
};

}  // namespace nova::gfx