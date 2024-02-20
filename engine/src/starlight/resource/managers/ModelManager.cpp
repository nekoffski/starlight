#include "ModelManager.h"

#include "starlight/core/utils/Log.h"
#include "starlight/resource/configs/ModelConfig.h"

namespace sl {

ModelManager::ModelManager(
  ResourcePools& resourcePools, MaterialManager& materialManager,
  MeshManager& meshManager
) :
    ResourceManager("Model"),
    m_resourcePools(resourcePools), m_materialManager(materialManager),
    m_meshManager(meshManager), m_models("Model", maxModels) {}

ModelManager::~ModelManager() {
    forEach([&]([[maybe_unused]] u64, Model* model) { destroyInternals(model); });
}

Model* ModelManager::load(const std::string& name) {
    auto config = ModelConfig::loadOBJ(name);

    if (not config) [[unlikely]] {
        LOG_ERROR("Could not load model: '{}'", name);
        return nullptr;
    }

    auto model = m_models.create();

    for (auto& material : config->materials) m_materialManager.load(material);

    for (const auto& meshConfig : config->meshes) {
        auto mesh = m_meshManager.load(meshConfig);
        auto material =
          m_materialManager.acquire(config->meshToMaterial[meshConfig.name]);
        model->addMesh(material, mesh);
    }

    return storeResource(name, model->getId(), model);
}

void ModelManager::destroyInternals(Model* model) {
    model->forEachMaterial([&]([[maybe_unused]] Material*, std::span<Mesh*> meshes) {
        for (auto& mesh : meshes) m_meshManager.destroy(mesh);
    });
}

}  // namespace sl
