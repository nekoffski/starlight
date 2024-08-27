#include "starlight/core/window/WindowManager.hh"

#include <gtest/gtest.h>

#include "mock/WindowMock.hh"

#include "starlight/core/event/EventBroker.hh"
#include "starlight/core/event/Quit.hh"

using namespace sl;
using namespace testing;

struct WindowManagerTests : Test {
    NiceMock<WindowMock> window;
};

TEST_F(WindowManagerTests, givenWindowManager_whenCreating_shouldSetOnKeyCallback) {
    EXPECT_CALL(window, onKeyCallback(_)).Times(1);
    WindowManager WindowManager{ &window };
}

TEST_F(
  WindowManagerTests, givenWindowManager_whenCreating_shouldSetOnMouseCallback
) {
    EXPECT_CALL(window, onMouseCallback(_)).Times(1);
    WindowManager WindowManager{ &window };
}

TEST_F(
  WindowManagerTests, givenWindowManager_whenCreating_shouldSetOnWindowCloseCallback
) {
    EXPECT_CALL(window, onWindowCloseCallback(_)).Times(1);
    WindowManager WindowManager{ &window };
}

struct CallbacksTests : WindowManagerTests {
    void SetUp() override { called = false; }

    EventBroker eventBroker;
    bool called;
};

TEST_F(
  CallbacksTests,
  givenWindowManager_whenKeyboardInteractionDetected_shouldEmitMouseEvent
) {
    EXPECT_CALL(window, onMouseCallback)
      .Times(1)
      .WillOnce([](Window::OnMouseCallback callback) {
          callback(MouseAction::press, 1);
      });

    [[maybe_unused]] auto id = eventBroker.getProxy().pushEventHandler<MouseEvent>(
      [&]([[maybe_unused]] const MouseEvent& event) {
          EXPECT_EQ(event.action, MouseAction::press);
          EXPECT_EQ(event.button, 1);
          called = true;

          return EventChainBehaviour::propagate;
      }
    );

    WindowManager windowManager{ &window };

    eventBroker.dispatch();
    EXPECT_TRUE(called);
}

TEST_F(CallbacksTests, givenWindowManager_whenWindowCloses_shouldEmitQuitEvent) {
    [[maybe_unused]] auto id = eventBroker.getProxy().pushEventHandler<QuitEvent>(
      [&]([[maybe_unused]] const auto&) {
          called = true;
          return EventChainBehaviour::propagate;
      }
    );

    EXPECT_CALL(window, onWindowCloseCallback)
      .Times(1)
      .WillOnce([](Window::OnWindowCloseCallback callback) { callback(); });

    WindowManager windowManager{ &window };

    eventBroker.dispatch();
    EXPECT_TRUE(called);
}

TEST_F(
  CallbacksTests,
  givenWindowManager_whenKeyboardInteractionDetected_shouldEmitKeyEvent
) {
    EXPECT_CALL(window, onKeyCallback)
      .Times(1)
      .WillOnce([](Window::OnKeyCallback callback) { callback(KeyAction::press, 1); }
      );

    [[maybe_unused]] auto id = eventBroker.getProxy().pushEventHandler<KeyEvent>(
      [&]([[maybe_unused]] const KeyEvent& event) {
          EXPECT_EQ(event.action, KeyAction::press);
          EXPECT_EQ(event.key, 1);
          called = true;
          return EventChainBehaviour::propagate;
      }
    );

    WindowManager windowManager{ &window };

    eventBroker.dispatch();
    EXPECT_TRUE(called);
}
