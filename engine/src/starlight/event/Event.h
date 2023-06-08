#pragma once

#include <kc/event/EventManager.hpp>
#include <kc/event/EventObserver.h>
#include <kc/event/EventWrapper.hpp>

namespace sl::event {

using EventWrapper  = kc::event::EventWrapper;
using EventObserver = kc::event::EventObserver;
using EventManager  = kc::event::EventManager;

}  // namespace sl::event