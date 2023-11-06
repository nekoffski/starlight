#pragma once

#include <array>
#include <span>
#include <functional>

#include "starlight/core/math/Glm.h"
#include "starlight/core/math/Vertex2.h"
#include "starlight/core/math/Vertex3.h"
#include "starlight/core/utils/Id.h"

#include "starlight/renderer/Geometry.h"
#include "starlight/renderer/Material.h"
#include "starlight/renderer/Texture.h"
#include "starlight/renderer/Shader.h"
#include "starlight/renderer/fwd.h"

#include "GeometryRenderData.h"
#include "GlobalState.h"

namespace sl {

// TODO: change to objects or something
constexpr int builtinRenderPassWorld = 1;
constexpr int builtinRenderPassUI    = 2;

struct RendererBackend {
    enum class BultinRenderPass { world, ui };

    virtual ~RendererBackend() = default;

    virtual u32 getRenderPassId(const std::string& renderPass) const = 0;

    virtual bool beginFrame(float deltaTime) = 0;
    virtual bool endFrame(float deltaTime)   = 0;

    bool renderPass(uint8_t id, auto&& callback) {
        if (beginRenderPass(id)) {
            callback();
            return endRenderPass(id);
        }
        return false;
    }

    virtual std::unique_ptr<Shader::Impl> createShaderImpl(Shader&) = 0;

    virtual bool beginRenderPass(uint8_t id) = 0;
    virtual bool endRenderPass(uint8_t id)   = 0;

    virtual void renderUI(std::function<void()>&&) = 0;

    virtual void drawGeometry(const GeometryRenderData& modelMatrix) = 0;

    virtual void onViewportResize(uint32_t width, uint32_t height) = 0;

    virtual TextureLoader* getTextureLoader() const = 0;

    virtual void acquireGeometryResources(
      Geometry& geometry, uint32_t vertexSize, uint32_t vertexCount,
      void* vertexData, std::span<uint32_t> indices
    ) = 0;

    virtual void releaseGeometryResources(Geometry& geometry) = 0;
};

}  // namespace sl
