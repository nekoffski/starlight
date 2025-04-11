#include "MeshComponent.hh"

#include "starlight/app/factories/MeshFactory.hh"

namespace sl {

void MeshComponent::Parser::deserialize(Entity& entity, const nlohmann::json& json)
  const {
    const auto& meshName = json["mesh"].get<std::string>();
    entity.add<MeshComponent>(MeshFactory::get().find(meshName));
}

nlohmann::json MeshComponent::Parser::serialize(const MeshComponentData& data
) const {
    nlohmann::json root;
    root["mesh"] = data.mesh->name;
    return root;
}

std::string MeshComponent::Parser::getComponentName() const {
    return "MeshComponent";
}

}  // namespace sl
