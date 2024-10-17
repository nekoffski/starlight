#pragma once

#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/core/math/Core.hh"
#include "starlight/core/utils/Id.hh"
#include "starlight/core/utils/Resource.hh"

#include "RenderTarget.hh"
#include "CommandBuffer.hh"

#include "fwd.hh"

namespace sl {

class RenderPass : public NonMovable, public Identificable<RenderTarget> {
public:
    static constexpr u8 clearNone          = 0x0;
    static constexpr u8 clearColorBuffer   = 0x1;
    static constexpr u8 clearDepthBuffer   = 0x2;
    static constexpr u8 clearStencilBuffer = 0x4;

    struct Properties {
        Rect2u32 rect;
        Vec4<f32> clearColor;
        u8 clearFlags;

        bool hasPreviousPass;
        bool hasNextPass;

        std::vector<RenderTarget> renderTargets;
    };

    static OwningPtr<RenderPass> create(
      RendererBackend& renderer, const Properties& props
    );

    explicit RenderPass(const Properties& props);
    virtual ~RenderPass() = default;

    template <typename C>
    requires Callable<C, void, CommandBuffer&, u8>
    void run(CommandBuffer& commandBuffer, u8 attachmentIndex, C&& callback) {
        begin(commandBuffer, attachmentIndex);
        callback(commandBuffer, attachmentIndex);
        end(commandBuffer);
    }

    void setClearColor(const Vec4<f32>& color);
    void setRect(const Rect2u32& extent);
    void setRectSize(const Vec2<u32>& size);

    virtual void regenerateRenderTargets(
      const std::vector<RenderTarget>& renderTargets
    ) = 0;

private:
    virtual void begin(CommandBuffer& commandBuffer, u8 attachmentIndex) = 0;
    virtual void end(CommandBuffer& commandBuffer)                       = 0;

protected:
    Properties m_props;
};

}  // namespace sl
