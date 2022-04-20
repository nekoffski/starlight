#pragma once

#include <gmock/gmock.h>

#include "sl/gfx/RenderApi.h"

using namespace sl::gfx;

struct RenderApiMock : RenderApi {
    MOCK_METHOD(void, init, ());
    MOCK_METHOD(void, setViewport, (const Viewport&));
    MOCK_METHOD(void, clearBuffers, (unsigned int buffers));

    MOCK_METHOD(void, setCullFace, (unsigned int), (override));

    MOCK_METHOD(void, drawArrays, (unsigned, unsigned, unsigned), (override));
    MOCK_METHOD(void, drawElements, (unsigned, unsigned, unsigned), (override));
    MOCK_METHOD(void, clearColor, (float, float, float, float), (override));

    MOCK_METHOD(void, depthMask, (bool), (override));
    MOCK_METHOD(void, setPolygonMode, (unsigned), (override));

    MOCK_METHOD(void, setDepthFunc, (unsigned), (override));
    MOCK_METHOD(void, setBlendFunc, (unsigned, unsigned), (override));

    MOCK_METHOD(void, enable, (unsigned), (override));
    MOCK_METHOD(void, disable, (unsigned), (override));

    MOCK_METHOD(sl::glob::RendererInfo, getRendererInfo, (), (const, override));
};