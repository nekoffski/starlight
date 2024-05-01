#include "FirstPersonCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "starlight/core/Log.h"
#include "starlight/core/window/Keys.h"

namespace sl {

FirstPersonCamera::FirstPersonCamera(const Properties& props) :
    m_position(props.position), m_front(0.0f, 0.0f, 1.0f), m_up(0.0f, 1.0f, 0.0f),
    m_right(1.0f, 0.0f, 0.0f), m_yaw(0.0f), m_pitch(0.0f), m_speed(10.0f),
    m_viewportWidth(props.viewportWidth), m_viewportHeight(props.viewportHeight) {}

Mat4f FirstPersonCamera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, worldUp);
}

Mat4f FirstPersonCamera::getProjectionMatrix() const {
    // TODO: make it configurable
    return glm::perspective(
      glm::radians(45.0f),
      static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight),
      0.1f, 1000.0f
    );
}

Vec3f FirstPersonCamera::getPosition() const { return m_position; }

void FirstPersonCamera::update(float deltaTime) {
    processInput(m_speed * deltaTime);

    m_front = glm::normalize(Vec3f(
      std::cos(m_yaw) * std::cos(m_pitch), std::sin(m_pitch),
      std::sin(m_yaw) * std::cos(m_pitch)
    ));

    m_right = glm::normalize(glm::cross(m_front, worldUp));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

void FirstPersonCamera::onViewportResize(u32 w, u32 h) {
    m_viewportWidth  = w;
    m_viewportHeight = h;
}

void FirstPersonCamera::processInput(const float speed) {
    auto& window = WindowManager::get();

    if (window.isKeyPressed(SL_KEY_W)) m_position += speed * m_front;
    if (window.isKeyPressed(SL_KEY_S)) m_position -= speed * m_front;
    if (window.isKeyPressed(SL_KEY_D)) m_position += speed * m_right;
    if (window.isKeyPressed(SL_KEY_A)) m_position -= speed * m_right;

    const auto mouseDelta = window.getMousePositionDelta() * 0.001f;
    m_pitch -= mouseDelta.y;
    m_yaw += mouseDelta.x;

    if (window.isKeyPressed(SL_KEY_M)) m_speed += 0.5f;
    if (window.isKeyPressed(SL_KEY_N)) m_speed -= 0.5f;
}

}  // namespace sl
