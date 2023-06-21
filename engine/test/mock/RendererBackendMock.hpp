#pragma once

#include "starlight/renderer/RendererBackend.h"

#include <gmock/gmock.h>

struct RendererBackendMock : sl::RendererBackend {
    MOCK_METHOD(bool, beginFrame, (float), (override));
    MOCK_METHOD(bool, endFrame, (float), (override));
    MOCK_METHOD(void, onViewportResize, (uint32_t, uint32_t), (override));
    MOCK_METHOD(void, updateGlobalState, (const sl::GlobalState& globalState), (override));
    MOCK_METHOD(void, drawGeometry, (const sl::GeometryRenderData& modelMatrix), (override));
    MOCK_METHOD(sl::TextureLoader*, getTextureLoader, (), (const, override));
    MOCK_METHOD(void, acquireMaterialResources, (sl::Material & material), (override));
    MOCK_METHOD(void, releaseMaterialResources, (sl::Material & material), (override));
    MOCK_METHOD(
        void, acquireGeometryResources,
        (sl::Geometry & geometry, std::span<sl::Vertex3> vertices, std::span<uint32_t> indices),
        (override)
    );
    MOCK_METHOD(void, releaseGeometryResources, (sl::Geometry & geometry), (override));
};
