#pragma once

#include "starlight/core/math/Vertex.hh"

#include "starlight/renderer/gpu/Shader.hh"
#include "starlight/renderer/gpu/Texture.hh"
#include "starlight/renderer/gpu/Mesh.hh"
#include "starlight/renderer/gpu/RenderPass.hh"
#include "starlight/renderer/gpu/RenderTarget.hh"

#include "starlight/renderer/fwd.hh"

namespace sl {

struct ResourcePools {
    virtual ~ResourcePools() = default;

    virtual RenderTarget* createRenderTarget(
      const RenderTarget::Properties& props, RenderPass* renderPass
    )                                                            = 0;
    virtual void destroyRenderTarget(RenderTarget& renderTarget) = 0;

    virtual RenderPass* createRenderPass(const RenderPass::Properties& props) = 0;
    virtual void destroyRenderPass(RenderPass& renderPass)                    = 0;
};

}  // namespace sl