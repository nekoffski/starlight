#pragma once

#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include <starlight/core/Error.h>
#include <starlight/core/event/IEventObserver.h>
#include <starlight/core/types/NotNullPtr.hpp>

namespace starl::core::event {

class EventBus {
public:
    void notifyAll(std::shared_ptr<Event>);
    void notify(std::shared_ptr<Event>, const std::vector<EventObserverIdent>&);
    void registerObserver(const types::NotNullPtr<IEventObserver>&, EventObserverIdent);

private:
    std::unordered_map<EventObserverIdent, IEventObserver*> m_observers;
};
} // namespace starl::core::event
