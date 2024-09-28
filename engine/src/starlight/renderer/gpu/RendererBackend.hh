#pragma once

#include <array>
#include <span>
#include <functional>

#include "starlight/core/math/Core.hh"
#include "starlight/core/math/Vertex.hh"
#include "starlight/core/math/Vertex.hh"

#include "starlight/renderer/fwd.hh"
#include "starlight/renderer/Material.hh"
#include "starlight/renderer/gpu/Mesh.hh"
#include "starlight/renderer/gpu/Texture.hh"
#include "starlight/renderer/gpu/Shader.hh"
#include "starlight/renderer/gpu/CommandBuffer.hh"

namespace sl {

struct RendererBackend : public NonCopyable, public NonMovable {
    virtual ~RendererBackend() = default;

    template <typename C>
    requires Callable<C> u64 renderFrame(float deltaTime, C&& callback) {
        if (beginFrame(deltaTime)) {
            callback();
            endFrame(deltaTime);
        }
        return getRenderedVertexCount();
    }

    virtual void setViewport(const Rect2<u32>& viewport) = 0;

    virtual u64 getRenderedVertexCount() const = 0;

    virtual bool beginFrame(float deltaTime) = 0;
    virtual bool endFrame(float deltaTime)   = 0;

    virtual void drawMesh(const Mesh& mesh) = 0;

    virtual void onViewportResize(const Vec2<u32>& viewportSize) = 0;

    virtual CommandBuffer& getCommandBuffer()       = 0;
    virtual u32 getImageIndex()                     = 0;
    virtual Texture* getSwapchainTexture(u32 index) = 0;
    virtual Texture* getDepthTexture()              = 0;
};

}  // namespace sl
