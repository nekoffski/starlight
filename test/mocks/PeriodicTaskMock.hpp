#pragma once

#include <gmock/gmock.h>

#include "sl/async/PeriodicTask.h"

struct PeriodicTaskMock : sl::async::PeriodicTask {
    MOCK_METHOD(bool, shouldInvoke, (), ());
    MOCK_METHOD(void, invoke, (), ());
    MOCK_METHOD(std::string, getName, (), (const));
};
