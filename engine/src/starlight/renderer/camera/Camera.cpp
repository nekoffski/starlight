#include "Camera.hh"

#include "starlight/core/event/EventProxy.hh"
#include "starlight/core/event/WindowResized.hh"

namespace sl {

Camera::Camera() : m_eventSentinel(EventProxy::get()) {
    EventProxy::get().pushEventHandler<WindowResized>(
      [&](const auto& event) -> EventChainBehaviour {
          m_viewportSize = event.size;
          return EventChainBehaviour::propagate;
      },
      m_eventSentinel
    );
}

}  // namespace sl
