#pragma once

#include <unordered_map>
#include <vector>

#include <starlight/core/event/IEventObserver.h>
#include <starlight/core/types/NotNullPtr.hpp>

namespace starl::core::event {

class EventBus {
public:
    void notifyAll(const Event&);
    void notify(const Event&, const std::vector<EventObserverIdent>&);
    void registerObserver(const types::NotNullPtr<IEventObserver>&, EventObserverIdent);

private:
    std::unordered_map<EventObserverIdent, IEventObserver*> m_observers;
};

}
