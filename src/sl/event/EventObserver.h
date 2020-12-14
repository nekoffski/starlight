#pragma once

#include "Event.h"
#include "EventPool.h"

namespace sl::event {

struct EventObserver {
    virtual void handleEvents(EventPool&) = 0;
};
}
