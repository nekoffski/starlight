#include "starlight/core/Engine.h"

#include <memory>

#include <gtest/gtest.h>

#include "mock/ApplicationMock.hpp"
#include "mock/WindowMock.hpp"
#include "mock/RendererBackendMock.hpp"
#include "mock/TextureLoaderMock.hpp"

#include "starlight/event/Event.h"
#include "starlight/event/Quit.h"

using namespace testing;

struct EngineTests : Test {
    NiceMock<ApplicationMock> application;
    NiceMock<WindowMock> window;
    NiceMock<RendererBackendMock> rendererBackend;
    NiceMock<TextureLoaderMock> textureLoader;

    std::unique_ptr<sl::core::Engine> engine;

    void SetUp() override {
        ON_CALL(rendererBackend, getTextureLoader).WillByDefault(Return(&textureLoader));
        sl::platform::Platform platform(&window, &rendererBackend);
        engine = std::make_unique<sl::core::Engine>(platform);
    }

    void setupSingleEngineIteration() {
        EXPECT_CALL(application, isRunning).Times(2).WillOnce(Return(true)).WillOnce(Return(false));
    }

    void setupZeroEngineIterations() { EXPECT_CALL(application, isRunning).Times(0); }
};

TEST_F(EngineTests, givenApplicationThatStops_whenRunningEngine_shouldStop) {
    EXPECT_CALL(application, isRunning).Times(1).WillOnce(Return(false));

    engine->run(application);
}

TEST_F(EngineTests, givenApplication_whenRunningEngine_shouldCallUpdate) {
    setupSingleEngineIteration();

    EXPECT_CALL(application, update(_)).Times(1);

    engine->run(application);
}

TEST_F(EngineTests, givenEngine_whenEmittingQuitEvent_shouldQuit) {
    setupZeroEngineIterations();

    sl::event::EventManager::get().emitEvent<sl::event::QuitEvent>("Test");
    engine->run(application);
}

TEST_F(EngineTests, givenEngine_whenReceivedSignal_shouldQuit) {
    setupZeroEngineIterations();

    kill(getpid(), SIGINT);
    engine->run(application);
}
