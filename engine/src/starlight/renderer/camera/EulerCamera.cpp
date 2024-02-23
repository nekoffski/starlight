#include "EulerCamera.h"

#include "starlight/core/window/WindowManager.h"
#include "starlight/core/window/Keys.h"

namespace sl {

EulerCamera::EulerCamera(const Properties& props) :
    m_target(props.target), m_radius(props.radius), m_pitch(90.0f), m_yaw(90.0f),
    m_viewportWidth(props.viewportWidth), m_viewportHeight(props.viewportHeight) {
    recalculateVectors();
}

Mat4f EulerCamera::getViewMatrix() const { return m_viewMatrix; }

Mat4f EulerCamera::getProjectionMatrix() const {
    return glm::perspective(
      glm::radians(45.0f),
      static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight),
      0.1f, 1000.0f
    );
}

Vec3f EulerCamera::getPosition() const { return m_position; }

void EulerCamera::update(float deltaTime) {
    static constexpr float speed = 150.0f;

    processInput(speed * deltaTime);
    truncateCoefficients();
    recalculateVectors();
    updateViewMatrix();
}

void EulerCamera::onScroll(float offset) { m_radius -= 2.5f * offset; }

void EulerCamera::onViewportResize(u32 w, u32 h) {
    m_viewportWidth  = w;
    m_viewportHeight = h;
}

void EulerCamera::processInput(const float speed) {
    auto windowManager = WindowManager::getPtr();

    if (windowManager->isKeyPressed(SL_KEY_W)) m_pitch -= speed;
    if (windowManager->isKeyPressed(SL_KEY_S)) m_pitch += speed;
    if (windowManager->isKeyPressed(SL_KEY_D)) m_yaw -= speed;
    if (windowManager->isKeyPressed(SL_KEY_A)) m_yaw += speed;

    if (windowManager->isKeyPressed(SL_KEY_1)) m_radius += speed / 5.0f;
    if (windowManager->isKeyPressed(SL_KEY_2)) m_radius -= speed / 5.0f;

    if (windowManager->isMouseButtonPressed(SL_MOUSE_BUTTON_2)) {
        windowManager->hideCursor();

        const float scaledSpeed = speed / 10.0f;
        const auto delta = scaledSpeed * windowManager->getMousePositionDelta();

        m_pitch -= delta.y;
        m_yaw += delta.x;
    } else {
        windowManager->showCursor();
    }
}

void EulerCamera::truncateCoefficients() {
    if (m_yaw >= 360.0f) m_yaw = 0.0f;
    if (m_yaw < 0.0f) m_yaw = 360.0f;

    if (m_pitch > 170.0f) m_pitch = 170.0f;
    if (m_pitch < 10.0f) m_pitch = 10.0f;

    static constexpr float maxRadius = 100.0f;

    if (m_radius < 0.0f) m_radius = 0.0f;
    if (m_radius > maxRadius) m_radius = maxRadius;
}

void EulerCamera::recalculateVectors() {
    const static Vec3f worldUp = Vec3f{ 0.0f, 1.0f, 0.0f };

    const auto pitchRadians = glm::radians(m_pitch);
    const auto yawRadians   = glm::radians(m_yaw);

    // clang-format off
    m_position = m_radius * Vec3f {
        std::sin(pitchRadians) * std::cos(yawRadians),
        std::cos(pitchRadians),
        std::sin(pitchRadians) * std::sin(yawRadians)
    };
    // clang-format on

    m_front = glm::normalize(m_target - m_position);
    m_right = glm::cross(m_front, worldUp);
    m_up    = glm::cross(m_right, m_front);
}

void EulerCamera::updateViewMatrix() {
    m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
}

}  // namespace sl