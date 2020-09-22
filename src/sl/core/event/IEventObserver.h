#pragma once

#include <memory>

#include "Event.h"

namespace sl::core::event {

using EventObserverIdent = std::string;

class IEventObserver {
public:
    virtual void onEvent(std::shared_ptr<Event>) = 0;
};
}