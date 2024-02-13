#pragma once

#include <unordered_map>

#include "starlight/core/Core.h"

#include "starlight/renderer/gpu/Mesh.h"
#include "starlight/renderer/gpu/ResourcePools.h"

#include "ResourceManager.hpp"
#include "MaterialManager.h"

#include "starlight/resource/resources/MeshConfig.h"

namespace sl {

class MeshManager : public ResourceManager<Mesh, MeshManager> {
public:
    explicit MeshManager(
      ResourcePools& resourcePools, MaterialManager& materialManager
    );
    ~MeshManager() override;

    Mesh* load(const MeshConfig auto& props) {
        if (auto mesh = createMesh(props); not mesh) [[likely]] {
            return storeResource(props.name, mesh->getId(), mesh);
        } else {
            LOG_ERROR("Could not create mesh");
            return nullptr;
        }
    }

    Mesh* getDefault3D();
    Mesh* getDefault2D();

private:
    void destroyInternals(Mesh* resource) override;
    std::string getResourceName() const override;

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

    Mesh* m_defaultMesh3D;
    Mesh* m_defaultMesh2D;
};

}  // namespace sl
