#include "EventBus.hh"

namespace sl {

EventBus::~EventBus() { m_broker->unregister(m_uuid); }

EventBus::EventBus(std::shared_ptr<details::EventBroker> eventBroker)
    : m_uuid(RandomEngine::get().uuid()), m_broker(std::move(eventBroker)) {}

}  // namespace sl