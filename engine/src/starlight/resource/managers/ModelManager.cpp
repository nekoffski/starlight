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

    for (auto& material : config->materials) m_materialManager.load(material);

    std::vector<Mesh*> meshes;
    meshes.reserve(config->meshes.size());

    std::transform(
      config->meshes.begin(), config->meshes.end(), std::back_inserter(meshes),
      [&](MeshConfig3D& config) -> Mesh* { return m_meshManager.load(config); }
    );

    auto model = m_models.create(meshes);
    return storeResource(name, model->getId(), model);
}

void ModelManager::destroyInternals(Model* model) {
    for (auto& mesh : model->getMeshes()) m_meshManager.destroy(mesh);
    m_models.destroy(model->getId());
}

}  // namespace sl
