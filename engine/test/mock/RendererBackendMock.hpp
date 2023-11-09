#pragma once

#include "starlight/renderer/gpu/RendererBackend.h"

#include <gmock/gmock.h>

using namespace sl;

struct RendererBackendMock : RendererBackend {
    MOCK_METHOD(u32, getRenderPassId, (const std::string&), (const));
    MOCK_METHOD(bool, beginFrame, (float));
    MOCK_METHOD(bool, endFrame, (float));
    MOCK_METHOD(std::unique_ptr<Shader::Impl>, createShaderImpl, (Shader&));
    MOCK_METHOD(bool, beginRenderPass, (uint8_t));
    MOCK_METHOD(bool, endRenderPass, (uint8_t));
    MOCK_METHOD(void, renderUI, (std::function<void()> &&));
    MOCK_METHOD(void, drawGeometry, (const GeometryRenderData&));
    MOCK_METHOD(void, onViewportResize, (uint32_t, uint32_t));
    MOCK_METHOD(Texture*, createTexture, (const Texture::Properties&, const void*));
    MOCK_METHOD(void, destroyTexture, (Texture&));
    MOCK_METHOD(
      Geometry*, createGeometry,
      (const Geometry::Properties&, const Geometry::Data&), (override)
    );
    MOCK_METHOD(void, destroyGeometry, (Geometry&), (override));
};
