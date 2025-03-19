#include "Model.hh"

#include "starlight/physx/bvh/AABB.hh"

namespace sl {

class CustomModel : public Model {
public:
    using Model::Model;

    void addSub(SharedPtr<Mesh> mesh, SharedPtr<Material> material);

    Type getType() const override;
    nlohmann::json serialize() const override;

    const BoundingVolume* getBoundingVolume() const;

private:
    AABB m_boundingBox;
};

}  // namespace sl
