#pragma once

#include <array>
#include <span>
#include <functional>

#include "starlight/core/math/Core.h"
#include "starlight/core/math/Vertex.h"
#include "starlight/core/math/Vertex.h"

#include "starlight/renderer/fwd.h"
#include "starlight/renderer/Material.h"
#include "starlight/renderer/gpu/Mesh.h"
#include "starlight/renderer/gpu/Texture.h"
#include "starlight/renderer/gpu/Shader.h"
#include "starlight/renderer/gpu/CommandBuffer.h"

#include "RendererBackendProxy.h"

namespace sl {

struct RendererBackend {
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
    virtual bool beginFrame(float deltaTime)   = 0;
    virtual bool endFrame(float deltaTime)     = 0;

    virtual void drawMesh(const Mesh& mesh) = 0;

    virtual void onViewportResize(const Vec2<u32>& viewportSize) = 0;

    virtual RendererBackendProxy* getProxy()  = 0;
    virtual ResourcePools* getResourcePools() = 0;
};

}  // namespace sl
