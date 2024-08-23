#include "ModelManager.hh"

#include "starlight/core/Log.hh"
#include "starlight/resource/configs/ModelConfig.hh"

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

    for (const auto& materialConfig : config->materials) {
        auto material = m_materialManager.load(materialConfig);
        for (auto& meshConfig : config->meshes[material->getName()]) {
            auto mesh = m_meshManager.load(meshConfig);
            model->addMesh(material, mesh);
        }
    }
    return storeResource(name, model->getId(), model);
}

void ModelManager::destroyInternals(Model* model) {
    model->forEachMesh([&](Mesh* mesh) { m_meshManager.destroy(mesh); });
}

}  // namespace sl
