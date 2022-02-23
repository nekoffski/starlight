#pragma once

#include <gmock/gmock.h>

#include "sl/core/Window.h"

using namespace sl::core;

struct WindowMock : public Window {
    explicit WindowMock() : Window({}, "windowTitle") {}

    MOCK_METHOD(void, setResizeCallback, (ResizeCallback), (override));
    MOCK_METHOD(void, init, (), (override));
    MOCK_METHOD(void, update, (float), (override));
    MOCK_METHOD(bool, getShouldClose, (), (const, override));
    MOCK_METHOD(void, setShouldClose, (bool), (override));
    MOCK_METHOD(void, changeCursorState, (bool), (override));
    MOCK_METHOD(void, enableCursor, (), (override));
    MOCK_METHOD(void, disableCursor, (), (override));
    MOCK_METHOD(void*, getHandle, (), (const, override));
    MOCK_METHOD(void, makeContextCurrent, (), (override));
    MOCK_METHOD(void, swapBuffers, (), (override));
    MOCK_METHOD(math::Size2D, getSize, (), (const, override));
};
