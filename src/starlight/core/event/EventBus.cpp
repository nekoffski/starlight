#include <starlight/core/event/EventBus.h>

namespace starl::core::event {
    
    err::Error EventBus::notifyAll(std::shared_ptr<Event> event) {
        for (auto &[_, observer] : m_observers) {
            observer->onEvent(event);
            if (event->isHandled) {
                break;
            }
        }
        return err::NO_ERROR;
    }

    std::queue<err::Error> EventBus::notify(std::shared_ptr<Event> event, const std::vector<EventObserverIdent>& targets) {
        std::queue<err::Error> errors;
        
        for (auto& target : targets) {
            if (auto observer = m_observers.find(target); observer != m_observers.end()) {
                observer->second->onEvent(event);
            } else {
                errors.push(makeErr(ErrorCode::TARGET_NOT_EXIST, err::Severity::ERROR, target));
            }
            if (event->isHandled) {
                break;
            }
        }
        return errors;
    }

    err::Error EventBus::registerObserver(const types::NotNullPtr<IEventObserver>& observer, EventObserverIdent ident) {
        if (auto o = m_observers.find(ident); o != m_observers.end()) {
            return makeErr(ErrorCode::TWO_OBSERVERS_WITH_SAME_IDENT, err::Severity::FATAL);
        }
        m_observers[ident] = observer; 
        return err::NO_ERROR;
    }
 
}
