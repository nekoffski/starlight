#pragma once

#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "IEventObserver.h"
#include "starlight/core/Error.h"
#include "starlight/misc/types/NotNullPtr.hpp"

namespace sl::core::event {

class EventBus {
public:
    void notifyAll(std::shared_ptr<Event>);
    void notify(std::shared_ptr<Event>, const std::vector<EventObserverIdent>&);
    void registerObserver(const misc::types::NotNullPtr<IEventObserver>&, EventObserverIdent);

private:
    std::unordered_map<EventObserverIdent, IEventObserver*> m_observers;
};
} // namespace sl::core::event
