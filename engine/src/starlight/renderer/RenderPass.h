#pragma once

#include <vector>

#include <starlight/core/Core.h>
#include <starlight/core/math/Glm.h>

#include "RenderTarget.h"

namespace sl {

class RenderPass {
public:
    static constexpr u8 clearNone          = 0x0;
    static constexpr u8 clearColorBuffer   = 0x1;
    static constexpr u8 clearDepthBuffer   = 0x2;
    static constexpr u8 clearStencilBuffer = 0x4;

private:
    u32 m_id;

    Vec4f m_renderArea;
    Vec4f m_clearColor;

    u8 m_clearFlags;

    std::vector<RenderTarget*> m_renderTargets;
};

}  // namespace sl
