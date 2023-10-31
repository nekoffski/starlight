#include "FirstPersonCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "starlight/core/window/Keys.h"

namespace sl {

FirstPersonCamera::FirstPersonCamera(const Properties& props) :
    m_position(props.position), m_front(0.0f, 0.0f, 1.0f), m_up(0.0f, 1.0f, 0.0f),
    m_right(1.0f, 0.0f, 0.0f), m_yaw(0.0f), m_pitch(0.0f) {}

Mat4f FirstPersonCamera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, worldUp);
}

Mat4f FirstPersonCamera::getProjectionMatrix() const {
    // TODO: make it configurable
    return glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 1000.0f);
}

Vec3f FirstPersonCamera::getPosition() const { return m_position; }

void FirstPersonCamera::update(float deltaTime) {
    static constexpr float speed = 10.0f;
    processInput(speed * deltaTime);

    m_front = glm::normalize(Vec3f(
      std::cos(m_yaw) * std::cos(m_pitch), std::sin(m_pitch),
      std::sin(m_yaw) * std::cos(m_pitch)
    ));

    m_right = glm::normalize(glm::cross(m_front, worldUp));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

void FirstPersonCamera::processInput(const float speed) {
    if (WindowManager::get().isKeyPressed(SL_KEY_W)) m_position += speed * m_front;
    if (WindowManager::get().isKeyPressed(SL_KEY_S)) m_position -= speed * m_front;
    if (WindowManager::get().isKeyPressed(SL_KEY_D)) m_position += speed * m_right;
    if (WindowManager::get().isKeyPressed(SL_KEY_A)) m_position -= speed * m_right;

    const auto lookSpeed = speed * 0.05f;

    if (WindowManager::get().isKeyPressed(SL_KEY_UP)) m_pitch += lookSpeed;
    if (WindowManager::get().isKeyPressed(SL_KEY_DOWN)) m_pitch -= lookSpeed;
    if (WindowManager::get().isKeyPressed(SL_KEY_RIGHT)) m_yaw += lookSpeed;
    if (WindowManager::get().isKeyPressed(SL_KEY_LEFT)) m_yaw -= lookSpeed;
}

}  // namespace sl
