#pragma once

#include <vector>

#include "fwd.hh"
#include "Core.hh"

namespace sl {

class EventHandlerSentinel {
    friend class EventProxy;

public:
    explicit EventHandlerSentinel(EventProxy& eventProxy);
    ~EventHandlerSentinel();

private:
    void addHandlerId(const EventHandlerId& id);

    EventProxy& m_eventProxy;
    std::vector<EventHandlerId> m_handlerIds;
};

}  // namespace sl
