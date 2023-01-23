#pragma once

#include <array>

#include "nova/core/Id.h"
#include "nova/math/Glm.h"
#include "Texture.h"

namespace nova::gfx {

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
    core::Id objectId;
    math::mat4 model;

    std::array<Texture*, 16> textures;

    float deltaTime;
};

struct ObjectUniformObject {
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
    virtual void updateObject(const GeometryRenderData& modelMatrix) = 0;

    virtual void onViewportResize(uint32_t width, uint32_t height) = 0;

    virtual TextureLoader* getTextureLoader() const = 0;
};

}  // namespace nova::gfx
