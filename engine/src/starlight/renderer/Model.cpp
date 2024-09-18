#include "Model.hh"

#include "starlight/renderer/Material.hh"

namespace sl {

Model::Model(u64 id) : m_id(id), m_transform(nullptr) {}

void Model::addMesh(Material* material, Mesh* mesh) {
    const auto materialId = material->getId();

    m_materials[materialId] = material;
    m_materialIdToMeshes[materialId].push_back(mesh);
}

Mat4<f32> Model::getModelMatrix() {
    return m_transform ? m_transform->getWorld() : identityMatrix;
}

u64 Model::getId() const { return m_id; }

}  // namespace sl
