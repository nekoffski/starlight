#include <starlight/core/event/EventBus.h>

namespace starl::core::event {
    
    void EventBus::notifyAll(const Event& event) {
        for (auto &[_, observer] : m_observers) {
            observer->onEvent(event);
        }
    }

    void EventBus::notify(const Event& event, const std::vector<EventObserverIdent>& targets) {
        for (auto& target : targets) {
            if (auto observer = m_observers.find(target); observer != m_observers.end()) {
                observer->second->onEvent(event);
            } else {
                // TODO: handle error
            }
            
        }
    }

    void EventBus::registerObserver(const types::NotNullPtr<IEventObserver>& observer, EventObserverIdent ident) {
        if (auto o = m_observers.find(ident); o != m_observers.end()) {
            // TODO: error code handling
            return;
        }
        m_observers[ident] = observer; 
    }
 
}
