#pragma once

#include "sl/platform/IO.h"

#include "InputMock.hpp"
#include "WindowMock.hpp"

struct FakeIO : sl::platform::IO {
    explicit FakeIO()
        : IO(std::make_unique<WindowMock>(), std::make_unique<InputMock>()) {
        windowMock = static_cast<WindowMock*>(this->window.get());
        inputMock = static_cast<InputMock*>(this->input.get());
    }

    inline static WindowMock* windowMock;
    inline static InputMock* inputMock;
};