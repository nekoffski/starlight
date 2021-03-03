#pragma once

#include <xvent/Event.h>

#define DECLARE_EVENT(Event, Category) struct Event : xvent::EventBase<Event, Category>
