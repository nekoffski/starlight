#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <queue>

#include <starlight/core/event/Error.h>
#include <starlight/core/event/IEventObserver.h>
#include <starlight/core/types/NotNullPtr.hpp>

namespace starl::core::event {

class EventBus {
public:
    [[nodiscard]] err::Error notifyAll(std::shared_ptr<Event>);
    [[nodiscard]] std::queue<err::Error> notify(std::shared_ptr<Event>, const std::vector<EventObserverIdent>&);
    [[nodiscard]] err::Error registerObserver(const types::NotNullPtr<IEventObserver>&, EventObserverIdent);

private:
    std::unordered_map<EventObserverIdent, IEventObserver*> m_observers;
};

}
