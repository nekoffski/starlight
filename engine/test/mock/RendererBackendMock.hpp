#pragma once

#include "starlight/renderer/RendererBackend.h"

#include <gmock/gmock.h>

using namespace sl;

struct RendererBackendMock : RendererBackend {
    MOCK_METHOD(bool, beginFrame, (float));
    MOCK_METHOD(bool, endFrame, (float));
    MOCK_METHOD(bool, beginRenderPass, (uint8_t));
    MOCK_METHOD(u64, endRenderPass, (uint8_t));
    MOCK_METHOD(void, renderUI, (std::function<void()> &&));
    MOCK_METHOD(void, drawGeometry, (const GeometryRenderData&));
    MOCK_METHOD(void, onViewportResize, (uint32_t, uint32_t));
    MOCK_METHOD(ResourcePools*, getResourcePools, ());
    MOCK_METHOD(RendererBackendProxy*, getProxy, ());
};
