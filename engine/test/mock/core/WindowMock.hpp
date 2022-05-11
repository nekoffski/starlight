#pragma once

#include "nova/core/Window.h"

#include <gmock/gmock.h>

struct WindowMock : nova::core::Window {
    MOCK_METHOD(std::string_view, getVendor, (), (const, override));
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, swapBuffers, (), (override));
    MOCK_METHOD(nova::math::Size2i, getSize, (), (const, override));
};