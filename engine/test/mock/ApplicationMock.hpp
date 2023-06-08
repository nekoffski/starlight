#pragma once

#include "starlight/core/Application.h"

#include <gmock/gmock.h>

struct ApplicationMock : sl::core::Application {
    MOCK_METHOD(bool, isRunning, (), (const, override));
    MOCK_METHOD(void, update, (float), (override));
};
