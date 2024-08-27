#include "EventProxy.hh"

namespace sl {

EventProxy::EventProxy(details::Events& events, details::EventHandlers& handlers) :
    m_events(events), m_handlers(handlers) {}

void EventProxy::popEventHandler(const EventHandlerId id) {
    const auto condition = [id](const auto& record) -> bool {
        return record.getId() == id;
    };

    for (auto& chain : m_handlers | std::views::values)
        if (std::erase_if(chain, condition) != 0) return;

    LOG_WARN("Event handler with id='{}' not found", id);
}

}  // namespace sl
