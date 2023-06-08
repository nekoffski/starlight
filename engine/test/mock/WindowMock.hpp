#pragma once

#include "starlight/core/Window.h"

#include <gmock/gmock.h>

struct WindowMock : sl::core::Window {
    MOCK_METHOD(std::string_view, getVendor, (), (const, override));
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, swapBuffers, (), (override));
    MOCK_METHOD(sl::math::Size2u32, getSize, (), (const, override));
    MOCK_METHOD(void, onKeyCallback, (OnKeyCallback), (override));
    MOCK_METHOD(void, onMouseCallback, (OnMouseCallback), (override));
    MOCK_METHOD(void, onWindowCloseCallback, (OnWindowCloseCallback), (override));
    MOCK_METHOD(void, onWindowResizeCallback, (OnWindowResizeCallback), (override));
    MOCK_METHOD(sl::math::Vec2f, getMousePosition, (), (const, override));
    MOCK_METHOD(bool, isKeyPressed, (sl::core::Window::Key keyCode), (const, override));
};
