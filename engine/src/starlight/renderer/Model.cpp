#include "Model.h"

namespace sl {

Model::Model(u64 id, std::span<Mesh*> meshes) :
    m_id(id), m_meshes(meshes.begin(), meshes.end()), m_transform(nullptr) {}

Mat4f Model::getModelMatrix() {
    return m_transform ? m_transform->getWorld() : identityMatrix;
}

u64 Model::getId() const { return m_id; }

std::span<Mesh*> Model::getMeshes() { return m_meshes; }

}  // namespace sl
