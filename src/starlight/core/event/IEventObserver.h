#pragma once

#include <memory>

#include <starlight/core/event/Event.h>

namespace starl::core::event {

using EventObserverIdent = std::string;

class IEventObserver {
public:
    virtual void onEvent(std::shared_ptr<Event>) = 0;
};

}