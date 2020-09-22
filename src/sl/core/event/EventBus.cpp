#include "EventBus.h"

#include "sl/core/log/Logger.h"

namespace sl::core::event {

void EventBus::notifyAll(std::shared_ptr<Event> event) {
    SL_DEBUG("invoking notify all");
    for (auto& [_, observer] : m_observers) {
        observer->onEvent(event);
        if (event->isHandled) {
            break;
        }
    }
}

void EventBus::notify(std::shared_ptr<Event> event, const std::vector<EventObserverIdent>& targets) {
    SL_DEBUG("invoking notify");
    for (auto& target : targets) {
        if (auto observer = m_observers.find(target); observer != m_observers.end()) {
            observer->second->onEvent(event);
        }
        if (event->isHandled) {
            break;
        }
    }
}

void EventBus::registerObserver(const misc::types::NotNullPtr<IEventObserver>& observer, EventObserverIdent ident) {
    SL_INFO("registering event observer: {}", ident);
    if (auto o = m_observers.find(ident); o != m_observers.end()) {
        throw CoreException(ErrorCode::TWO_OBSERVERS_WITH_SAME_IDENT);
    }
    m_observers[ident] = observer;
}
} // namespace sl::core::event
