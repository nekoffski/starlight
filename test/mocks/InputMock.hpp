#pragma once

#include "sl/core/Input.h"

#include <gmock/gmock.h>

using namespace sl::core;

struct InputMock : public Input {
    struct Factory : public Input::Factory {
        MOCK_METHOD(std::shared_ptr<Input>, create, (types::NotNullPtr<void>), (override));

        inline static Factory* instance = nullptr;
    };

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, setKeyCallback, (InputCallback), (override));
    MOCK_METHOD(bool, isKeyPressed, (int), (const, override));
    MOCK_METHOD(bool, isMouseButtonPressed, (int), (const, override));
    MOCK_METHOD((std::pair<double, double>), getMousePosition, (), (const, override));
    MOCK_METHOD((std::pair<double, double>), getMousePositonDelta, (), (const, override));
    MOCK_METHOD(double, getScrollDelta, (), (const, override));
};
