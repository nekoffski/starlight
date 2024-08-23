#pragma once

#include <unordered_map>

#include "starlight/core/Core.hh"

#include "starlight/renderer/gpu/Mesh.hh"
#include "starlight/renderer/gpu/ResourcePools.hh"

#include "ResourceManager.hh"
#include "MaterialManager.hh"

#include "starlight/resource/configs/MeshConfig.hh"

namespace sl {

class MeshManager : public ResourceManager<Mesh, MeshManager> {
public:
    explicit MeshManager(
      ResourcePools& resourcePools, MaterialManager& materialManager
    );
    ~MeshManager();

    Mesh* load(const MeshConfig auto& props) {
        if (auto mesh = createMesh(props); mesh) [[likely]] {
            return storeResource(props.name, mesh->getId(), mesh);
        } else {
            LOG_ERROR("Could not create me.hh");
            return nullptr;
        }
    }

    Mesh* getDefault3D();
    Mesh* getDefault2D();

private:
    void destroyInternals(Mesh* resource) override;

    void createDefaultGeometries();

    Mesh* createMesh(const MeshConfig auto& config) {
        return m_resourcePools.createMesh(
          Mesh::Properties{ config.name }, config.vertices, config.indices,
          config.calculateExtent()
        );
    }

    ResourcePools& m_resourcePools;
    MaterialManager& m_materialManager;

    Mesh* m_defaultMesh3D;
    Mesh* m_defaultMesh2D;
};

}  // namespace sl
