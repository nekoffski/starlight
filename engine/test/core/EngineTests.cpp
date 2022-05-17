#include "nova/core/Engine.h"

#include <gtest/gtest.h>

#include "mock/core/ApplicationMock.hpp"
#include "mock/core/WindowMock.hpp"

#include "nova/event/Event.h"
#include "nova/event/Quit.h"

using namespace testing;

struct EngineTests : Test {
    NiceMock<ApplicationMock> application;
    NiceMock<WindowMock> window;

    nova::core::Engine engine{nova::platform::Platform{&window}};

    void setupSingleEngineIteration() {
        EXPECT_CALL(application, isRunning).Times(2).WillOnce(Return(true)).WillOnce(Return(false));
    }

    void setupZeroEngineIterations() { EXPECT_CALL(application, isRunning).Times(0); }
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

TEST_F(EngineTests, givenEngine_whenEmittingQuitEvent_shouldQuit) {
    setupZeroEngineIterations();

    nova::event::EventManager::get().emitEvent<nova::event::QuitEvent>("Test");
    engine.run(application);
}

TEST_F(EngineTests, givenEngine_whenReceivedSignal_shouldQuit) {
    setupZeroEngineIterations();

    kill(getpid(), SIGINT);
    engine.run(application);
}
