#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Event.h"
#include "sl/core/log/Logger.h"

namespace sl::event {

using EventsVector = std::vector<std::shared_ptr<Event>>;
using EventsMap = std::unordered_map<EventCategory, EventsVector>;

class EventPool {
public:
    explicit EventPool(EventsMap& eventsMap)
        : m_eventsMap(eventsMap) {}

    EventsVector getEventsByCategory(std::vector<EventCategory> categories) {
        EventsVector events;

		for (auto& category : categories) {
			if (m_eventsMap.contains(category)) {
				auto categoryEvents = m_eventsMap[category];
				events.insert(events.end(), categoryEvents.begin(), categoryEvents.end());
			}
		}

        return events;
    }

private:
    EventsVector m_emptyEventsVector;
    EventsMap& m_eventsMap;
};
}
