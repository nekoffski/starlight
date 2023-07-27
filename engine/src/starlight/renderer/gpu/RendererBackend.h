#pragma once

#include <array>
#include <span>

#include "starlight/core/math/Vertex3.h"
#include "starlight/core/Id.h"
#include "starlight/core/math/Glm.h"

#include "starlight/renderer/Texture.h"
#include "starlight/renderer/Material.h"
#include "starlight/renderer/Geometry.h"
#include "starlight/renderer/fwd.h"

#include "GlobalState.h"
#include "GeometryRenderData.h"

namespace sl {

// TODO: change to objects or something
constexpr int builtinRenderPassWorld = 1;
constexpr int builtinRenderPassUI    = 2;

struct RendererBackend {
    enum class BultinRenderPass { world, ui };

    virtual ~RendererBackend() {}

    virtual bool beginFrame(float deltaTime) = 0;
    virtual bool endFrame(float deltaTime)   = 0;

    virtual void updateGlobalWorldState(const GlobalState& globalState)          = 0;
    virtual void updateGlobalUIState(Mat4f projection, Mat4f view, int32_t mode) = 0;

    bool renderPass(uint8_t id, auto&& callback) {
        if (beginRenderPass(id)) {
            callback();
            return endRenderPass(id);
        }
        return false;
    }

    virtual bool beginRenderPass(uint8_t id) = 0;
    virtual bool endRenderPass(uint8_t id)   = 0;

    virtual void drawGeometry(const GeometryRenderData& modelMatrix) = 0;

    virtual void onViewportResize(uint32_t width, uint32_t height) = 0;

    virtual TextureLoader* getTextureLoader() const = 0;

    // TODO: check for errors in acquire functions
    virtual void acquireMaterialResources(Material& material) = 0;
    virtual void releaseMaterialResources(Material& material) = 0;

    virtual void acquireGeometryResources(
        Geometry& geometry, std::span<Vertex3> vertices, std::span<uint32_t> indices
    )                                                         = 0;
    virtual void releaseGeometryResources(Geometry& geometry) = 0;
};

}  // namespace sl
