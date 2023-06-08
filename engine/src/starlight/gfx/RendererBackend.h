#pragma once

#include <array>
#include <span>

#include "starlight/math/Vertex3.h"
#include "starlight/core/Id.h"
#include "starlight/math/Glm.h"
#include "Texture.h"
#include "Material.h"
#include "Geometry.h"

#include "fwd.h"

namespace sl::gfx {

struct GlobalUniformObject {
    math::Mat4f projection;
    math::Mat4f view;
    // pad to 256
    math::Mat4f p0, p1;
};

struct GlobalState {
    math::Mat4f projectionMatrix;
    math::Mat4f viewMatrix;
    math::Vec3f viewPosition;
    math::Vec4f ambientColor;

    int32_t mode;
};

struct GeometryRenderData {
    math::mat4 model;
    Geometry* geometry;
};

struct MaterialUniformObject {
    math::Vec4f diffuseColor;  // 16 bytes
    math::Vec4f reserved0;     // 16 bytes, reserved for future use
    math::Vec4f reserved1;     // 16 bytes, reserved for future use
    math::Vec4f reserved2;     // 16 bytes, reserved for future use
};

struct RendererBackend {
    virtual ~RendererBackend() {}

    virtual bool beginFrame(float deltaTime) = 0;
    virtual bool endFrame(float deltaTime)   = 0;

    virtual void updateGlobalState(const GlobalState& globalState)   = 0;
    virtual void drawGeometry(const GeometryRenderData& modelMatrix) = 0;

    virtual void onViewportResize(uint32_t width, uint32_t height) = 0;

    virtual TextureLoader* getTextureLoader() const = 0;

    // TODO: check for errors in acquire functions
    virtual void acquireMaterialResources(gfx::Material& material) = 0;
    virtual void releaseMaterialResources(gfx::Material& material) = 0;

    virtual void acquireGeometryResources(
        Geometry& geometry, std::span<math::Vertex3> vertices, std::span<uint32_t> indices
    )                                                         = 0;
    virtual void releaseGeometryResources(Geometry& geometry) = 0;
};

}  // namespace sl::gfx
