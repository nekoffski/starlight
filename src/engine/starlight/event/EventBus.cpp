#include "EventBus.hh"

namespace sl {

EventBus::~EventBus() {
    if (m_broker) {
        m_broker->unregister(m_uuid);
    }
}

EventBus::EventBus(EventBus&& oth)
    : m_broker(std::exchange(oth.m_broker, nullptr)),
      m_uuid(std::exchange(oth.m_uuid, Str{})) {}

EventBus::EventBus(std::shared_ptr<details::EventBroker> eventBroker)
    : m_uuid(RandomEngine::get().uuid()), m_broker(std::move(eventBroker)) {}

}  // namespace sl