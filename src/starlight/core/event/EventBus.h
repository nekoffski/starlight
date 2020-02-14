#pragma once

#include <unordered_map>
#include <vector>

#include <starlight/core/event/IEventObserver.h>

namespace starl::core::event {

class EventBus {
public:
    void notifyAll(const Event&);
    void notify(const Event&, const std::vector<EventObserverIdent>&);
    void registerObserver(IEventObserver*, EventObserverIdent);

private:
    std::unordered_map<EventObserverIdent, IEventObserver*> m_observers;
};

}
