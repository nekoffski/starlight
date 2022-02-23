#include <gtest/gtest.h>

#include "sl/async/Timer.h"

using namespace sl;
using namespace testing;

TEST(TimerTests, givenTimerWithPositiveSleepTime_whenAsyncSleep_shouldReturnTrue) {
    EXPECT_TRUE(async::Timer{0.1f}.asyncSleep());
}

TEST(TimerTests, givenTimerWithNegativeOrZeroSleepTime_whenAsyncSleep_shouldReturnTrue) {
    EXPECT_FALSE(async::Timer{0.0f}.asyncSleep());
    EXPECT_FALSE(async::Timer{-1.0f}.asyncSleep());
}
