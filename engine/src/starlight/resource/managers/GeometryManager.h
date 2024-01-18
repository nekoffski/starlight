#pragma once

#include <unordered_map>

#include "starlight/core/Core.h"

#include "starlight/renderer/gpu/Geometry.h"
#include "starlight/renderer/gpu/ResourcePools.h"

#include "MaterialManager.h"

#include "starlight/resource/resources/GeometryConfig.h"

namespace sl {

// TODO: pass objects as views instead of references
class GeometryManager {
    static constexpr int maxGeometries = 4096;

public:
    explicit GeometryManager(
      ResourcePools& resourcePools, MaterialManager& materialManager
    );
    ~GeometryManager();

    Geometry* acquire(u32 id);
    Geometry* load(const GeometryConfig auto& props) {
        auto geometry = createGeometry(props);

        if (not geometry) {
            LOG_ERROR("Could not create geometry");
            return nullptr;
        }

        m_geometries[geometry->getId()] = geometry;
        return geometry;
    }

    void destroy(uint32_t id);
    void destroyAll();

    Geometry* getDefault3D();
    Geometry* getDefault2D();

private:
    void createDefaultGeometries();

    Geometry* createGeometry(const GeometryConfig auto& config) {
        auto material =
          m_materialManager.acquire(config.materialName)
            ?: m_materialManager.getDefaultMaterial();

        return m_resourcePools.createGeometry(
          Geometry::Properties{ config.name, material }, config.vertices,
          config.indices, config.calculateExtent()
        );
    }

    ResourcePools& m_resourcePools;
    MaterialManager& m_materialManager;

    std::unordered_map<u32, Geometry*> m_geometries;

    Geometry* m_defaultGeometry3D;
    Geometry* m_defaultGeometry2D;
};

}  // namespace sl
