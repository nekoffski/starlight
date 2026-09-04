#include "EventBroker.hh"

namespace sl::details {

void EventBroker::dispatch() {
    std::unique_lock lk{m_handlerMutex};

    auto allEvents = consume();

    for (const auto& [type, events] : allEvents) {
        const auto callbacks = m_callbacks.find(type);
        if (callbacks == m_callbacks.end()) {
            continue;
        }

        for (const auto& [_, handler] : callbacks->second) {
            for (const auto& event : events) {
                handler(event);
            }
        }
    }
}

void EventBroker::unregisterCallback(
    const Str& id, const std::type_index& type
) {
    std::unique_lock lk{m_handlerMutex};

    if (const auto callbacks = m_callbacks.find(type);
        callbacks != m_callbacks.end()) {
        callbacks->second.erase(id);
    }
}

void EventBroker::unregister(const Str& id) {
    std::unique_lock lk{m_handlerMutex};

    for (auto& [_, callbacks] : m_callbacks) {
        callbacks.erase(id);
    }
}

void EventBroker::post(const std::type_index& idx, const std::any& event) {
    std::unique_lock lk{m_eventMutex};
    m_events[idx].push_back(event);
}

EventBroker::EventMap EventBroker::consume() {
    EventMap events;
    {
        std::unique_lock lk{m_eventMutex};
        std::swap(events, m_events);
    }
    return events;
}

void EventBroker::registerCallback(
    const Str& id, const std::type_index& type, GenericCallback&& callback
) {
    std::unique_lock lk{m_handlerMutex};
    m_callbacks[type].emplace(id, std::move(callback));
}

}  // namespace sl::details
