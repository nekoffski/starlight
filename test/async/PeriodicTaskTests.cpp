#include <gtest/gtest.h>

#include "mocks/PeriodicTaskMock.hpp"

using namespace sl;
using namespace testing;

struct PeriodicTaskTests : Test {
    PeriodicTaskMock periodicTaskMock;

    async::PeriodicTask* periodicTask = &periodicTaskMock;
};

TEST_F(PeriodicTaskTests, givenPeriodicTask_shouldBeActive) { EXPECT_TRUE(periodicTask->isActive); }

TEST_F(PeriodicTaskTests,
       givenPeriodicTaskHandle_whenCallingDisable_shouldChangeStateOfPeriodicTask) {
    async::PeriodicTask::Handle taskHandle = periodicTask;

    ASSERT_TRUE(periodicTask->isActive);

    taskHandle.disable();

    EXPECT_FALSE(periodicTask->isActive);
}

TEST_F(PeriodicTaskTests, givenPeriodicTaskHandleWithNullptr_whenCallingDisable_shouldNotThrow) {
    async::PeriodicTask::Handle handle = nullptr;

    EXPECT_NO_THROW(handle.disable());
}
