#include <starlight/core/event/EventBus.h>

namespace starl::core::event {
    
    void EventBus::notifyAll(const Event& event) {

    }

    void EventBus::notify(const Event& event, const std::vector<EventObserverIdent>& targets) {

    }

    void EventBus::registerObserver(const types::NotNullPtr<IEventObserver>& observer, EventObserverIdent ident) {
        if (auto o = m_observers.find(ident); o != m_observers.end()) {
            // TODO: error code handling
            return;
        }
        m_observers[ident] = observer;
    }
 
}
