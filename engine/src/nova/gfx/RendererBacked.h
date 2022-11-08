#pragma once

#include "nova/math/Glm.h"

namespace nova::gfx {

struct GlobalUniformObject {
    math::Mat4f projection;
    math::Mat4f view;
    // pad to 256
    math::Mat4f p0, p1;
};

struct RendererBackend {
    virtual ~RendererBackend() {}

    virtual bool beginFrame(float deltaTime) = 0;
    virtual bool endFrame(float deltaTime)   = 0;

    virtual void onViewportResize(uint32_t width, uint32_t height) = 0;
};

}  // namespace nova::gfx
