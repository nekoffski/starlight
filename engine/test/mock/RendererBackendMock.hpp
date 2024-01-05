#pragma once

#include "starlight/renderer/gpu/RendererBackend.h"

#include <gmock/gmock.h>

using namespace sl;

struct RendererBackendMock : RendererBackend {
    MOCK_METHOD(bool, beginFrame, (float));
    MOCK_METHOD(bool, endFrame, (float));
    MOCK_METHOD(void, drawGeometry, (const Geometry&));
    MOCK_METHOD(void, onViewportResize, (uint32_t, uint32_t));
    MOCK_METHOD(ResourcePools*, getResourcePools, ());
    MOCK_METHOD(RendererBackendProxy*, getProxy, ());
};
