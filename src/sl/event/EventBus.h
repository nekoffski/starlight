#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Event.h"
#include "EventObserver.h"
#include "EventPool.h"

namespace sl::event {

class EventBus {
    using EventsObservers = std::vector<EventObserver*>;

public:
    static void spreadEvents() {
        EventPool eventPool{ m_eventsMap };

        for (auto& observer : m_eventsObservers)
            observer->handleEvents(eventPool);

        reset();
    }

    static void registerEventObserver(EventObserver* observer) {
        m_eventsObservers.push_back(observer);
    }

    template <class T, typename... Args>
    static void emitEvent(Args&&... args) {
        auto event = std::make_shared<T>(std::forward<Args>(args)...);
        m_eventsMap[event->getCategory()].push_back(event);
    }

private:
    static void reset() {
        m_eventsMap.clear();
    }

    inline static EventsObservers m_eventsObservers;
    inline static EventsMap m_eventsMap;
};
}
