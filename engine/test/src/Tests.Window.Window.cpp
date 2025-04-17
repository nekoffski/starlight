#include "starlight/window/Window.hh"

#include <gtest/gtest.h>

#include "utils/WindowMock.hh"

#include "starlight/event/EventBroker.hh"
#include "starlight/window/Events.hh"
#include "starlight/core/Globals.hh"

using namespace sl;
using namespace testing;

struct WindowTests : Test {
    EventBroker eventBroker;
    Globals globals{ {} };

    kstd::UniquePtr<NiceMock<WindowImplMock>> windowImpl =
      kstd::makeUnique<NiceMock<WindowImplMock>>();
};

TEST_F(WindowTests, onWindowCloseCallbackSet) {
    EXPECT_CALL(*windowImpl, onWindowCloseCallback(_)).Times(1);
    Window window{ std::move(windowImpl) };
}

struct WindowCallbacksTests : WindowTests {
    void SetUp() override { called = false; }
    bool called;
};

TEST_F(WindowCallbacksTests, closeWindow) {
    [[maybe_unused]] auto id = eventBroker.getProxy().pushEventHandler<QuitEvent>(
      [&]([[maybe_unused]] const auto&) { called = true; }
    );

    EXPECT_CALL(*windowImpl, onWindowCloseCallback)
      .Times(1)
      .WillOnce([](Window::Impl::OnWindowCloseCallback callback) { callback(); });

    Window window{ std::move(windowImpl) };

    eventBroker.dispatch();
    EXPECT_TRUE(called);
}
