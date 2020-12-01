#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Event.h"
#include "sl/core/log/Logger.h"

namespace sl::core::event {

using EventsVector = std::vector<std::shared_ptr<Event>>;
using EventsMap = std::unordered_map<EventCategory, EventsVector>;

class EventPool {
public:
    explicit EventPool(EventsMap& eventsMap)
        : m_eventsMap(eventsMap) {}

    EventsVector& getEventsByCategory(EventCategory category) {
        if (m_eventsMap.count(category) > 0)
            return m_eventsMap[category];
        return m_emptyEventsVector;
    }

private:
    EventsVector m_emptyEventsVector;
    EventsMap& m_eventsMap;
};
}
