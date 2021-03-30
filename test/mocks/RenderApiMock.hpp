#pragma once

#include "sl/gfx/RenderApi.h"

#include <gmock/gmock.h>

using namespace sl::gfx;

struct RenderApiMock : public RenderApi {
    struct Factory : public RenderApi::Factory {
        MOCK_METHOD(std::unique_ptr<RenderApi>, create, (), (override));
    };

    MOCK_METHOD(void, drawArrays, (unsigned, unsigned, unsigned), (override));
    MOCK_METHOD(void, drawElements, (unsigned, unsigned, unsigned), (override));
    MOCK_METHOD(void, clearColor, (float, float, float, float), (override));

    MOCK_METHOD(void, depthMask, (bool), (override));
    MOCK_METHOD(void, setPolygonMode, (unsigned), (override));

    MOCK_METHOD(void, setDepthFunc, (unsigned), (override));
    MOCK_METHOD(void, setBlendFunc, (unsigned, unsigned), (override));

    MOCK_METHOD(void, enable, (unsigned), (override));
    MOCK_METHOD(void, disable, (unsigned), (override));
};