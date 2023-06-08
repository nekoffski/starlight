#pragma once

#include "starlight/gfx/RendererBackend.h"

#include <gmock/gmock.h>

struct RendererBackendMock : sl::gfx::RendererBackend {
    MOCK_METHOD(bool, beginFrame, (float), (override));
    MOCK_METHOD(bool, endFrame, (float), (override));
    MOCK_METHOD(void, onViewportResize, (uint32_t, uint32_t), (override));
    MOCK_METHOD(void, updateGlobalState, (const sl::gfx::GlobalState& globalState), (override));
    MOCK_METHOD(void, drawGeometry, (const sl::gfx::GeometryRenderData& modelMatrix), (override));
    MOCK_METHOD(sl::gfx::TextureLoader*, getTextureLoader, (), (const, override));
    MOCK_METHOD(void, acquireMaterialResources, (sl::gfx::Material & material), (override));
    MOCK_METHOD(void, releaseMaterialResources, (sl::gfx::Material & material), (override));
    MOCK_METHOD(
        void, acquireGeometryResources,
        (sl::gfx::Geometry & geometry, std::span<sl::math::Vertex3> vertices,
         std::span<uint32_t> indices),
        (override)
    );
    MOCK_METHOD(void, releaseGeometryResources, (sl::gfx::Geometry & geometry), (override));
};
