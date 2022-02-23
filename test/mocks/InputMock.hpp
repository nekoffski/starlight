#pragma once

#include <gmock/gmock.h>

#include "sl/core/Input.h"

using namespace sl;

struct InputMock : core::Input {
    MOCK_METHOD(void, init, (void*), (override));
    MOCK_METHOD(bool, isKeyPressed, (int), (const, override));
    MOCK_METHOD(bool, isMouseButtonPressed, (int), (const, override));
    MOCK_METHOD(math::Vec2, getMousePosition, (), (const, override));
    MOCK_METHOD(void, setOnMousePositionChange, (sl::core::Mouse::Callback), (override));
    MOCK_METHOD(void, setOnMouseScroll, (sl::core::Mouse::Callback), (override));
};
