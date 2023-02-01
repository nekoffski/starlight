#include "EulerCamera.h"

#include "nova/core/WindowManager.h"

#include "nova/platform/Keys.h"

namespace nova::gfx {

EulerCamera::EulerCamera(const Properties& props)
    : m_target(props.target), m_radius(props.radius), m_pitch(90.0f), m_yaw(90.0f) {
    recalculateVectors();
}

math::Mat4f EulerCamera::getViewMatrix() const { return m_viewMatrix; }

math::Mat4f EulerCamera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 1000.0f);
}

math::Vec3f EulerCamera::getPosition() const { return m_position; }

void EulerCamera::update(float deltaTime) {
    static constexpr float speed = 100.0f;

    processInput(speed * deltaTime);
    truncateCoefficients();
    recalculateVectors();
    updateViewMatrix();
}

void EulerCamera::processInput(const float speed) {
    if (core::WindowManager::get().isKeyPressed(NOVA_KEY_W)) m_pitch -= speed;
    if (core::WindowManager::get().isKeyPressed(NOVA_KEY_S)) m_pitch += speed;
    if (core::WindowManager::get().isKeyPressed(NOVA_KEY_D)) m_yaw -= speed;
    if (core::WindowManager::get().isKeyPressed(NOVA_KEY_A)) m_yaw += speed;

    if (core::WindowManager::get().isKeyPressed(NOVA_KEY_1)) m_radius += speed / 10.0f;
    if (core::WindowManager::get().isKeyPressed(NOVA_KEY_2)) m_radius -= speed / 10.0f;
}

void EulerCamera::truncateCoefficients() {
    if (m_yaw >= 360.0f) m_yaw = 0.0f;
    if (m_yaw < 0.0f) m_yaw = 360.0f;

    if (m_pitch > 170.0f) m_pitch = 170.0f;
    if (m_pitch < 10.0f) m_pitch = 10.0f;

    static constexpr float maxRadius = 25.0f;

    if (m_radius < 0.0f) m_radius = 0.0f;
    if (m_radius > maxRadius) m_radius = maxRadius;
}

void EulerCamera::recalculateVectors() {
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

void EulerCamera::updateViewMatrix() { m_viewMatrix = glm::lookAt(m_position, m_target, m_up); }

}  // namespace nova::gfx