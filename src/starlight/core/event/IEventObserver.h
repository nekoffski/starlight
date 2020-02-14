#pragma once

#include <starlight/core/event/Event.h>

namespace starl::core::event {

using EventObserverIdent = std::string;

class IEventObserver {
public:
    virtual void onEvent(const Event&) = 0;
};

}