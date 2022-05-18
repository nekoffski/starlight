#pragma once

#include <gmock/gmock.h>

#include <kc/event/EventObserver.h>

using namespace kc::event;

struct EventObserverMock : EventObserver {
    MOCK_METHOD(void, onEvent, (EventWrapper & eventWrapper), (override));
};
