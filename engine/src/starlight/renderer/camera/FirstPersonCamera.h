#pragma once

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "starlight/core/math/Glm.h"
#include "starlight/core/window/WindowManager.h"
#include "starlight/core/window/Keys.h"

namespace sl {

class FirstPersonCamera : public Camera {
   public:
    explicit FirstPersonCamera()
        : m_position(0.0f, 0.0f, 1.0f)
        , m_front(0.0f, 0.0f, 1.0f)
        , m_up(0.0f, 1.0f, 0.0f)
        , m_right(1.0f, 0.0f, 0.0f) {}

    Mat4f getViewMatrix() const override { return glm::translate(identityMatrix, -m_position); }

    Mat4f getProjectionMatrix() const override {
        return glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 1000.0f);
    }

    Vec3f getPosition() const override { return m_position; }

    void update(float deltaTime) override {
        static constexpr float speed = 10.0f;

        processInput(speed * deltaTime);
    }

    void processInput(const float speed) {
        if (WindowManager::get().isKeyPressed(SL_KEY_W)) m_position -= speed * m_front;
        if (WindowManager::get().isKeyPressed(SL_KEY_S)) m_position += speed * m_front;
        if (WindowManager::get().isKeyPressed(SL_KEY_D)) m_position += speed * m_right;
        if (WindowManager::get().isKeyPressed(SL_KEY_A)) m_position -= speed * m_right;
    }

   private:
    Vec3f m_position;
    Vec3f m_front;
    Vec3f m_up;
    Vec3f m_right;
};

}  // namespace sl