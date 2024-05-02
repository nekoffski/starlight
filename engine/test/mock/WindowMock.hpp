#pragma once

#include "starlight/core/window/Window.h"

#include <gmock/gmock.h>

struct WindowMock : sl::Window {
    MOCK_METHOD(std::string_view, getVendor, (), (const, override));
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, showCursor, (), (override));
    MOCK_METHOD(void, hideCursor, (), (override));
    MOCK_METHOD(void, swapBuffers, (), (override));
    MOCK_METHOD(sl::Vec2u32, getSize, (), (const, override));
    MOCK_METHOD(sl::Vec2u32, getFramebufferSize, (), (const, override));
    MOCK_METHOD(void, onKeyCallback, (OnKeyCallback), (override));
    MOCK_METHOD(void, onMouseCallback, (OnMouseCallback), (override));
    MOCK_METHOD(void, onScrollCallback, (OnScrollCallback), (override));
    MOCK_METHOD(void, onWindowCloseCallback, (OnWindowCloseCallback), (override));
    MOCK_METHOD(void, onWindowResizeCallback, (OnWindowResizeCallback), (override));
    MOCK_METHOD(sl::Vec2f, getMousePosition, (), (const, override));
    MOCK_METHOD(bool, isKeyPressed, (sl::Window::Key keyCode), (const, override));
    MOCK_METHOD(
      bool, isMouseButtonPressed, (sl::Window::Button buttonCode), (const, override)
    );
};
