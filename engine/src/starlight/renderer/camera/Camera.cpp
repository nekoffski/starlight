#include "Camera.hh"

#include "starlight/core/event/EventProxy.hh"
#include "starlight/core/event/WindowResized.hh"

namespace sl {

Camera::Camera() : m_eventSentinel(EventProxy::get()) {
    m_eventSentinel.pushHandler<WindowResized>(
      [&](const auto& event) -> EventChainBehaviour {
          m_viewportSize = event.size;
          return EventChainBehaviour::propagate;
      }
    );
}

}  // namespace sl
