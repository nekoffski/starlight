#pragma once

#include "nova/core/Application.h"

#include <gmock/gmock.h>

struct ApplicationMock : nova::core::Application {
    MOCK_METHOD(bool, isRunning, (), (const, override));
    MOCK_METHOD(void, update, (float), (override));
};
