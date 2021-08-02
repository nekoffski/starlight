#pragma once

#include "sl/platform/IO.h"

#include "InputMock.hpp"
#include "WindowMock.hpp"

struct FakeIO : sl::platform::IO {
    explicit FakeIO()
        : IO(std::make_unique<WindowMock>(), std::make_unique<InputMock>()) {
    }
};