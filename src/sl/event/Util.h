#pragma once

#include <kc/event/Event.h>

#define DECLARE_EVENT(Event, Category) struct Event : kc::event::EventBase<Event, Category>
