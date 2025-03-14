#include "Model.hh"

namespace sl {

class CustomModel : public Model {
public:
    using Model::Model;

    void addSub(SharedPtr<Mesh> mesh, SharedPtr<Material> material);

    Type getType() const override;
    nlohmann::json serialize() const override;
};

}  // namespace sl
