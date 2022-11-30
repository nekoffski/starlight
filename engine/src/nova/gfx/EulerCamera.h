#pragma once

#include "Camera.h"

#include "nova/math/Glm.h"
#include "nova/core/WindowManager.h"

#include "nova/platform/Keys.h"

namespace nova::gfx {

class EulerCamera : public Camera {
   public:
    explicit EulerCamera()
        : m_position(0.0f)
        , m_front(0.0f, 0.0f, 1.0f)
        , m_right(1.0f, 0.0f, 0.0f)
        , m_up(0.0f, 1.0f, 0.0f) {}

    math::Mat4f getViewMatrix() const override {
        return glm::translate(math::identityMatrix, -m_position);
    }

    math::Mat4f getProjectionMatrix() const override {
        return glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 1000.0f);
    }

    math::Vec3f getPosition() const override { return m_position; }

    void update(float deltaTime) override {
        static constexpr float speed = 10.0f;
        const float scalledSpeed     = speed * deltaTime;

        if (core::WindowManager::get().isKeyPressed(NOVA_KEY_W))
            m_position -= m_front * scalledSpeed;

        if (core::WindowManager::get().isKeyPressed(NOVA_KEY_S))
            m_position += m_front * scalledSpeed;

        if (core::WindowManager::get().isKeyPressed(NOVA_KEY_D))
            m_position += m_right * scalledSpeed;

        if (core::WindowManager::get().isKeyPressed(NOVA_KEY_A))
            m_position -= m_right * scalledSpeed;
    }

   private:
    math::Vec3f m_position;

    math::Vec3f m_front;
    math::Vec3f m_right;
    math::Vec3f m_up;
};

}  // namespace nova::gfx