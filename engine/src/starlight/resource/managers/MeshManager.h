#pragma once

#include <unordered_map>

#include "starlight/core/Core.h"

#include "starlight/renderer/gpu/Mesh.h"
#include "starlight/renderer/gpu/ResourcePools.h"

#include "MaterialManager.h"

#include "starlight/resource/resources/MeshConfig.h"

namespace sl {

// TODO: pass objects as views instead of references
class MeshManager {
    static constexpr int maxGeometries = 4096;

public:
    explicit MeshManager(
      ResourcePools& resourcePools, MaterialManager& materialManager
    );
    ~MeshManager();

    Mesh* acquire(u32 id);
    Mesh* load(const MeshConfig auto& props) {
        auto mesh = createMesh(props);

        if (not mesh) {
            LOG_ERROR("Could not create mesh");
            return nullptr;
        }

        m_meshes[mesh->getId()] = mesh;
        return mesh;
    }

    void destroy(uint32_t id);
    void destroyAll();

    Mesh* getDefault3D();
    Mesh* getDefault2D();

private:
    void createDefaultGeometries();

    Mesh* createMesh(const MeshConfig auto& config) {
        auto material =
          m_materialManager.acquire(config.materialName)
            ?: m_materialManager.getDefaultMaterial();

        return m_resourcePools.createMesh(
          Mesh::Properties{ config.name, material }, config.vertices, config.indices,
          config.calculateExtent()
        );
    }

    ResourcePools& m_resourcePools;
    MaterialManager& m_materialManager;

    std::unordered_map<u32, Mesh*> m_meshes;

    Mesh* m_defaultMesh3D;
    Mesh* m_defaultMesh2D;
};

}  // namespace sl
