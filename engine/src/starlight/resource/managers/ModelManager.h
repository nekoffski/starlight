#pragma once

#pragma once

#include "starlight/core/Core.h"
#include "starlight/core/utils/ResourcePool.hpp"

#include "starlight/renderer/Model.h"

#include "MaterialManager.h"
#include "ResourceManager.hpp"
#include "MeshManager.h"

namespace sl {

class ModelManager : public ResourceManager<Model, ModelManager> {
    static constexpr u64 maxModels = 1024 * 4;

public:
    explicit ModelManager(
      ResourcePools& resourcePools, MaterialManager& materialManager,
      MeshManager& meshManager
    );
    ~ModelManager() override;

    Model* load(const std::string& name);

private:
    void destroyInternals(Model* resource) override;

    ResourcePools& m_resourcePools;
    MaterialManager& m_materialManager;
    MeshManager& m_meshManager;

    ResourcePool<Model> m_models;
};

}  // namespace sl
