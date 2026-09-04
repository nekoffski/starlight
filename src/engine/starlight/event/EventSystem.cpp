#include "EventSystem.hh"

namespace sl {

EventSystem::EventSystem()
    : m_broker{std::make_shared<details::EventBroker>()} {}

EventBus EventSystem::createBus() { return EventBus{m_broker}; }

void EventSystem::dispatch() { m_broker->dispatch(); }

}  // namespace sl
