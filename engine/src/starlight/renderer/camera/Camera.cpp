#include "Camera.hh"

#include "starlight/window/Events.hh"

namespace sl {

Camera::Camera(
  const Vec2<u32>& viewport, const Vec3<f32>& position,
  const ProjectionProperties& projectionProperties
)
    : m_position(position)
    , m_eventSentinel(EventProxy::get())
    , m_viewportSize(viewport)
    , m_projectionProperties(projectionProperties) {
    calculateProjectionMatrix();
    m_eventSentinel.add<ViewportChanged>([&](auto& event) {
        m_viewportSize = event.world.size;
        calculateProjectionMatrix();
    });
}

const Vec3<f32>& Camera::getPosition() const { return m_position; }

const Mat4<f32>& Camera::getViewMatrix() const { return m_viewMatrix; }

const Mat4<f32>& Camera::getInvViewMatrix() const { return m_invViewMatrix; }

const Mat4<f32>& Camera::getProjectionMatrix() const { return m_projectionMatrix; }

const Mat4<f32>& Camera::getInvProjectionMatrix() const {
    return m_invProjectionMatrix;
}

void Camera::setViewMatrix(const Mat4<f32>& viewMatrix) {
    m_viewMatrix    = viewMatrix;
    m_invViewMatrix = math::inverse(viewMatrix);
}

void Camera::calculateProjectionMatrix() {
    m_projectionMatrix = math::perspective(
      math::radians(m_projectionProperties.fov),
      static_cast<float>(m_viewportSize.w) / static_cast<float>(m_viewportSize.h),
      m_projectionProperties.nearZ, m_projectionProperties.farZ
    );
    m_invProjectionMatrix = math::inverse(m_projectionMatrix);
}

}  // namespace sl
