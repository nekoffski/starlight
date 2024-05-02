#pragma once

#include "starlight/renderer/gpu/RendererBackend.h"

#include <gmock/gmock.h>

using namespace sl;

struct RendererBackendMock : RendererBackend {
    MOCK_METHOD(bool, beginFrame, (float));
    MOCK_METHOD(void, setViewport, (const Viewport&));
    MOCK_METHOD(bool, endFrame, (float));
    MOCK_METHOD(void, drawMesh, (const Mesh&));
    MOCK_METHOD(void, onViewportResize, (const sl::Vec2<sl::u32>&));
    MOCK_METHOD(ResourcePools*, getResourcePools, ());
    MOCK_METHOD(RendererBackendProxy*, getProxy, ());
    MOCK_METHOD(u64, getRenderedVertexCount, (), (const, override));
};
