#pragma once

#include "nova/gfx/RendererBackend.h"

#include <gmock/gmock.h>

struct RendererBackendMock : nova::gfx::RendererBackend {
    MOCK_METHOD(bool, beginFrame, (float), (override));
    MOCK_METHOD(bool, endFrame, (float), (override));
    MOCK_METHOD(void, onViewportResize, (uint32_t, uint32_t), (override));
    MOCK_METHOD(void, updateGlobalState, (const nova::gfx::GlobalState& globalState), (override));
    MOCK_METHOD(void, drawGeometry, (const nova::gfx::GeometryRenderData& modelMatrix), (override));
    MOCK_METHOD(nova::gfx::TextureLoader*, getTextureLoader, (), (const, override));
    MOCK_METHOD(void, acquireMaterialResources, (nova::gfx::Material & material), (override));
    MOCK_METHOD(void, releaseMaterialResources, (nova::gfx::Material & material), (override));
    MOCK_METHOD(
        void, acquireGeometryResources,
        (nova::gfx::Geometry & geometry, std::span<nova::math::Vertex3> vertices,
         std::span<uint32_t> indices),
        (override)
    );
    MOCK_METHOD(void, releaseGeometryResources, (nova::gfx::Geometry & geometry), (override));
};
