#include "EventBroker.hh"

namespace sl {

EventBroker::EventBroker() : m_proxy(m_events, m_handlers) {
    m_events.reserve(defaultEventCapacity);
}

EventProxy& EventBroker::getProxy() { return m_proxy; }

void EventBroker::dispatch() {
    for (auto& event : m_events) {
        if (auto handlerRecord = m_handlers.find(event->getType());
            handlerRecord == m_handlers.end()) {
            break;
        } else {
            for (auto& handler : handlerRecord->second) {
                if (handler.callback(*event) == EventChainBehaviour::stop) break;
            }
        }
    }
    m_events.clear();
}

}  // namespace sl
