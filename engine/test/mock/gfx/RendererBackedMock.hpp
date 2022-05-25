#pragma once

#include "nova/gfx/RendererBacked.h"

#include <gmock/gmock.h>

struct RendererBackedMock : nova::gfx::RendererBackend {
    MOCK_METHOD(bool, beginFrame, (), (override));
    MOCK_METHOD(bool, endFrame, (), (override));
};
