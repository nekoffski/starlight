#include "starlight/core/window/WindowManager.h"

#include <gtest/gtest.h>

#include "mock/WindowMock.hpp"

#include "starlight/core/event/Event.h"
#include "starlight/core/event/Input.h"
#include "starlight/core/event/Quit.h"

using namespace sl;
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
    void SetUp() override { called = false; }

    EventManager eventManager;
    bool called;
};

TEST_F(
    WindowManagerCallbacksTests,
    givenWindowManager_whenKeyboardInteractionDetected_shouldEmitMouseEvent
) {
    EXPECT_CALL(window, onMouseCallback).Times(1).WillOnce([](Window::OnMouseCallback callback) {
        callback(MouseAction::press, 1);
    });

    eventManager.on<MouseEvent>([&]([[maybe_unused]] MouseEvent* event) {
        EXPECT_EQ(event->action, MouseAction::press);
        EXPECT_EQ(event->button, 1);
        called = true;
    });

    WindowManager windowManager{&window};

    eventManager.dispatch();
    EXPECT_TRUE(called);
}

TEST_F(WindowManagerCallbacksTests, givenWindowManager_whenWindowCloses_shouldEmitQuitEvent) {
    eventManager.on<QuitEvent>([&]([[maybe_unused]] auto) { called = true; });

    EXPECT_CALL(window, onWindowCloseCallback)
        .Times(1)
        .WillOnce([](Window::OnWindowCloseCallback callback) { callback(); });

    WindowManager windowManager{&window};

    eventManager.dispatch();
    EXPECT_TRUE(called);
}

TEST_F(
    WindowManagerCallbacksTests,
    givenWindowManager_whenKeyboardInteractionDetected_shouldEmitKeyEvent
) {
    EXPECT_CALL(window, onKeyCallback).Times(1).WillOnce([](Window::OnKeyCallback callback) {
        callback(KeyAction::press, 1);
    });

    eventManager.on<KeyEvent>([&]([[maybe_unused]] KeyEvent* event) {
        EXPECT_EQ(event->action, KeyAction::press);
        EXPECT_EQ(event->key, 1);
        called = true;
    });

    WindowManager windowManager{&window};

    eventManager.dispatch();
    EXPECT_TRUE(called);
}
