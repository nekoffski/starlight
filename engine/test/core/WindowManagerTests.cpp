#include "nova/core/WindowManager.h"

#include <gtest/gtest.h>

#include "mock/core/WindowMock.hpp"
#include "mock/EventObserverMock.hpp"

#include "nova/event/Event.h"
#include "nova/event/Input.h"
#include "nova/event/Quit.h"

using namespace nova;
using namespace nova::event;
using namespace nova::core;
using namespace testing;

struct WindowManagerTests : Test {
    NiceMock<WindowMock> window;
};

TEST_F(WindowManagerTests, givenWindowManager_whenCreating_shouldSetOnKeyCallback) {
    EXPECT_CALL(window, onKeyCallback(_)).Times(1);
    WindowManager WindowManager{&window};
}

TEST_F(WindowManagerTests, givenWindowManager_whenCreating_shouldSetOnMouseCallback) {
    EXPECT_CALL(window, onMouseCallback(_)).Times(1);
    WindowManager WindowManager{&window};
}

TEST_F(WindowManagerTests, givenWindowManager_whenCreating_shouldSetOnWindowCloseCallback) {
    EXPECT_CALL(window, onWindowCloseCallback(_)).Times(1);
    WindowManager WindowManager{&window};
}

struct WindowManagerCallbacksTests : WindowManagerTests {
    void SetUp() { eventManager.registerObserver(&eventObserver); }

    void TearDown() { eventManager.unregisterObserver(&eventObserver); }

    EventObserverMock eventObserver;
    EventManager eventManager;
};

TEST_F(WindowManagerCallbacksTests, givenWindowManager_whenWindowCloses_shouldEmitQuitEvent) {
    EXPECT_CALL(eventObserver, onEvent).Times(1).WillOnce([](EventWrapper& event) {
        ASSERT_TRUE(event.is<QuitEvent>());
    });

    EXPECT_CALL(window, onWindowCloseCallback)
        .Times(1)
        .WillOnce([](Window::OnWindowCloseCallback callback) { callback(); });

    WindowManager WindowManager{&window};
}

TEST_F(
    WindowManagerCallbacksTests,
    givenWindowManager_whenMouseInteractionDetected_shouldEmitMouseEvent
) {
    EXPECT_CALL(window, onMouseCallback).Times(1).WillOnce([](Window::OnMouseCallback callback) {
        callback(MouseAction::press, 1);
    });

    EXPECT_CALL(eventObserver, onEvent).Times(1).WillOnce([](EventWrapper& event) {
        ASSERT_TRUE(event.is<MouseEvent>());
    });

    WindowManager WindowManager{&window};
}

TEST_F(
    WindowManagerCallbacksTests,
    givenWindowManager_whenKeyboardInteractionDetected_shouldEmitKeyEvent
) {
    EXPECT_CALL(window, onKeyCallback).Times(1).WillOnce([](Window::OnKeyCallback callback) {
        callback(KeyAction::press, 1);
    });

    EXPECT_CALL(eventObserver, onEvent).Times(1).WillOnce([](EventWrapper& event) {
        ASSERT_TRUE(event.is<KeyEvent>());
    });

    WindowManager WindowManager{&window};
}