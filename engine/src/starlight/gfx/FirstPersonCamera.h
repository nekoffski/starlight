#pragma once

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "starlight/math/Glm.h"
#include "starlight/core/WindowManager.h"

#include "starlight/platform/Keys.h"

namespace sl::gfx {

class FirstPersonCamera : public Camera {
   public:
    explicit FirstPersonCamera()
        : m_position(0.0f, 0.0f, 1.0f)
        , m_front(0.0f, 0.0f, 1.0f)
        , m_up(0.0f, 1.0f, 0.0f)
        , m_right(1.0f, 0.0f, 0.0f) {}

    math::Mat4f getViewMatrix() const override {
        return glm::translate(math::identityMatrix, -m_position);
    }

    math::Mat4f getProjectionMatrix() const override {
        return glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 1000.0f);
    }

    math::Vec3f getPosition() const override { return m_position; }

    void update(float deltaTime) override {
        static constexpr float speed = 10.0f;

        processInput(speed * deltaTime);
    }

    void processInput(const float speed) {
        if (core::WindowManager::get().isKeyPressed(SL_KEY_W)) m_position -= speed * m_front;
        if (core::WindowManager::get().isKeyPressed(SL_KEY_S)) m_position += speed * m_front;
        if (core::WindowManager::get().isKeyPressed(SL_KEY_D)) m_position += speed * m_right;
        if (core::WindowManager::get().isKeyPressed(SL_KEY_A)) m_position -= speed * m_right;
    }

   private:
    math::Vec3f m_position;
    math::Vec3f m_front;
    math::Vec3f m_up;
    math::Vec3f m_right;
};

}  // namespace sl::gfx