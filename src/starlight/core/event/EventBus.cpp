#include <starlight/core/event/EventBus.h>

namespace starl::core::event {
    void EventBus::notifyAll(const Event&) {

    }

    void EventBus::notify(const Event&, const std::vector<EventObserverIdent>&) {

    }

    void EventBus::registerObserver(IEventObserver*, EventObserverIdent) {
        
    }
 
}
