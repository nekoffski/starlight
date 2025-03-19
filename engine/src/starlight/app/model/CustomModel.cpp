#include "CustomModel.hh"

namespace sl {

void CustomModel::addSub(SharedPtr<Mesh> mesh, SharedPtr<Material> material) {
    m_boundingBox.addExtent(mesh->getExtent());
    m_subs.emplace_back(std::move(mesh), std::move(material));
}

Model::Type CustomModel::getType() const { return Type::custom; }

nlohmann::json CustomModel::serialize() const { return nlohmann::json(); }

const BoundingVolume* CustomModel::getBoundingVolume() const {
    return &m_boundingBox;
}

}  // namespace sl
