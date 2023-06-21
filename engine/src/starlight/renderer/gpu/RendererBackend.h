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

#include "GlobalUniformObject.h"
#include "GlobalState.h"
#include "GeometryRenderData.h"
#include "MaterialUniformObject.h"

namespace sl {

struct RendererBackend {
    virtual ~RendererBackend() {}

    virtual bool beginFrame(float deltaTime) = 0;
    virtual bool endFrame(float deltaTime)   = 0;

    virtual void updateGlobalState(const GlobalState& globalState)   = 0;
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
