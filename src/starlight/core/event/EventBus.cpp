#include <starlight/core/event/EventBus.h>

#include <starlight/core/log/Logger.h>

namespace starl::core::event {

void EventBus::notifyAll(std::shared_ptr<Event> event) {
    LOG(DEBUG) << "invoking notify all";
    for (auto& [_, observer] : m_observers) {
        observer->onEvent(event);
        if (event->isHandled) {
            break;
        }
    }
}

void EventBus::notify(std::shared_ptr<Event> event, const std::vector<EventObserverIdent>& targets) {
    LOG(DEBUG) << "invoking notify";
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
    LOG(INFO) << "registering event observer: " << ident;
    if (auto o = m_observers.find(ident); o != m_observers.end()) {
        throw CoreException(ErrorCode::TWO_OBSERVERS_WITH_SAME_IDENT);
    }
    m_observers[ident] = observer;
}
} // namespace starl::core::event
