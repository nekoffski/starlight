#include "nova/core/Engine.h"

#include <gtest/gtest.h>

#include "mock/core/ApplicationMock.hpp"

using namespace testing;

struct EngineTests : Test {
    nova::core::Engine engine;
    testing::NiceMock<ApplicationMock> application;

    void setupSingleEngineIteration() {
        EXPECT_CALL(application, isRunning).Times(2).WillOnce(Return(true)).WillOnce(Return(false));
    }
};

TEST_F(EngineTests, givenApplicationThatStops_whenRunningEngine_shouldStop) {
    EXPECT_CALL(application, isRunning).Times(1).WillOnce(Return(false));

    engine.run(application);
}

TEST_F(EngineTests, givenApplication_whenRunningEngine_shouldCallUpdate) {
    setupSingleEngineIteration();

    EXPECT_CALL(application, update(_)).Times(1);

    engine.run(application);
}