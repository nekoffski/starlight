#pragma once

#include "starlight/core/Core.h"

#include "starlight/renderer/Geometry.h"
#include "starlight/renderer/gpu/RendererProxy.h"

#include "MaterialManager.h"

#include "starlight/resource/resources/GeometryConfig.h"

namespace sl {

// TODO: pass objects as views instead of references
class GeometryManager {
    static constexpr int maxGeometries = 4096;

public:
    explicit GeometryManager(
      RendererProxy& rendererProxy, MaterialManager& materialManager
    );

    Geometry* acquire(uint32_t id);

    Geometry* load(GeometryConfig auto& props) {
        auto geometry = findSlot();

        if (not geometry) {
            LOG_ERROR(
              "Could not find empty slot for geometry, consider changing configuration"
            );
            return nullptr;
        }

        if (not createGeometry(props, *geometry)) {
            LOG_ERROR("Could not create geometry");
            return nullptr;
        }

        return geometry;
    }

    void destroy(uint32_t id);
    void destroy(Geometry& geometry);

    Geometry* getDefault3D();
    Geometry* getDefault2D();

private:
    Geometry* findSlot();

    void invalidateEntry(Geometry& geometry);
    void createDefaultGeometries();

    bool createGeometry(GeometryConfig auto& props, Geometry& geometry) {
        geometry.name = props.name;

        m_rendererProxy.acquireGeometryResources(
          geometry, props.vertices, props.indices
        );  // TODO: check if succeed

        geometry.material =
          m_materialManager.acquire(props.materialName)
            ?: m_materialManager.getDefaultMaterial();

        return true;
    }

    RendererProxy& m_rendererProxy;
    MaterialManager& m_materialManager;
    std::array<Geometry, maxGeometries> m_geometries;

    Geometry m_defaultGeometry3D;
    Geometry m_defaultGeometry2D;
};

}  // namespace sl
