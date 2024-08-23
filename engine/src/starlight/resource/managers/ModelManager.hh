#pragma once

#pragma once

#include "starlight/core/Core.hh"
#include "starlight/core/ResourcePool.hh"

#include "starlight/renderer/Model.hh"

#include "MaterialManager.hh"
#include "ResourceManager.hh"
#include "MeshManager.hh"

namespace sl {

class ModelManager : public ResourceManager<Model, ModelManager> {
    static constexpr u64 maxModels = 1024 * 4;

public:
    explicit ModelManager(
      ResourcePools& resourcePools, MaterialManager& materialManager,
      MeshManager& meshManager
    );
    ~ModelManager();

    Model* load(const std::string& name);

private:
    void destroyInternals(Model* resource) override;

    ResourcePools& m_resourcePools;
    MaterialManager& m_materialManager;
    MeshManager& m_meshManager;

    ResourcePool<Model> m_models;
};

}  // namespace sl
