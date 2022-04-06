#include "Camera.h"

#include <kc/core/Log.h>

#include "sl/event/Event.h"
#include "sl/event/EventManager.h"

namespace sl::cam {

Camera::Camera(const gfx::ViewFrustum &viewFrustum, const math::Vec3 &position,
               const math::Vec3 &up, const math::Vec3 &front, const math::Vec3 &right)
    : EventListener("Camera"),
      viewFrustum(viewFrustum),
      m_position(std::move(position)),
      m_up(std::move(up)),
      m_front(std::move(front)),
      m_right(std::move(right)) {
    LOG_TRACE("Registering Camera as a event listener");
    sl::event::EventManager::get().registerListener(this);
}

Camera::~Camera() { sl::event::EventManager::get().unregisterListener(this); }

void Camera::handleEvents(const kc::event::EventProvider &eventProvider) {
    auto events = eventProvider.getByCategories<sl::event::CoreCategory>();

    for (auto &event : events) {
        if (event->is<sl::event::ChangeViewportEvent>()) {
            viewFrustum.viewport = event->asView<sl::event::ChangeViewportEvent>()->viewport;
            LOG_TRACE("Setting camera viewport: {}", toString(viewFrustum.viewport));

            calculateProjectionMatrix();
        }
    }
};

void Camera::onGui() {}

const math::Vec3 &Camera::getPosition() const { return m_position; }

const math::Vec3 &Camera::getUp() const { return m_up; }

const math::Vec3 &Camera::getFront() const { return m_front; }

const math::Vec3 &Camera::getRight() const { return m_right; }

const math::Mat4 &Camera::getProjectionMatrix() const { return m_projectionMatrix; }

const math::Mat4 Camera::getViewMatrix() const { return math::lookAt(m_position, m_front, m_up); }

void Camera::calculateProjectionMatrix() {
    const float aspect =
        static_cast<float>(viewFrustum.viewport.width) / viewFrustum.viewport.height;

    m_projectionMatrix =
        glm::perspective(viewFrustum.fieldOfView, aspect, viewFrustum.nearZ, viewFrustum.farZ);
}

}  // namespace sl::cam
