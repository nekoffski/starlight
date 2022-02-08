#include "Camera.h"

namespace sl::cam {

Camera::Camera(const gfx::ViewFrustum& viewFrustum, const math::Vec3& position,
    const math::Vec3& up, const math::Vec3& front, const math::Vec3& right)
    : viewFrustum(viewFrustum)
    , m_position(std::move(position))
    , m_up(std::move(up))
    , m_front(std::move(front))
    , m_right(std::move(right)) {
}

void Camera::onGui() {
}

const math::Vec3& Camera::getPosition() const {
    return m_position;
}

const math::Vec3& Camera::getUp() const {
    return m_up;
}

const math::Vec3& Camera::getFront() const {
    return m_front;
}

const math::Vec3& Camera::getRight() const {
    return m_right;
}

const math::Mat4& Camera::getProjectionMatrix() const {
    return m_projectionMatrix;
}

const math::Mat4 Camera::getViewMatrix() const {
    return math::lookAt(m_position, m_front, m_up);
}

}