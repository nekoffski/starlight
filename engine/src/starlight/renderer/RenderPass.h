#pragma once

#include <vector>

#include <starlight/core/Core.h>
#include <starlight/core/math/Glm.h>

#include "RenderTarget.h"

#include "fwd.h"

namespace sl {

class RenderPass {
public:
    static constexpr u8 clearNone          = 0x0;
    static constexpr u8 clearColorBuffer   = 0x1;
    static constexpr u8 clearDepthBuffer   = 0x2;
    static constexpr u8 clearStencilBuffer = 0x4;

    struct Properties {
        Vec4f area;
        Vec4f clearColor;
        u8 clearFlags;
        bool hasPreviousPass;
        bool hasNextPass;

        std::vector<RenderTarget::Properties> targets;
    };

    explicit RenderPass(u32 id, const Properties& props);
    virtual ~RenderPass() = default;

    void setClearColor(const Vec4f& color);
    void setArea(const Vec4f& area);
    void setAreaSize(u32 w, u32 h);

    u32 getId() const;

    virtual void regenerateRenderTargets(
      const std::vector<RenderTarget::Properties>& targets
    ) = 0;

protected:
    u32 m_id;
    Properties m_props;
};

}  // namespace sl
