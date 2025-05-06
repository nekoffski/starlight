#include "MeshComponent.hh"

#include "starlight/app/factories/MeshFactory.hh"
#include "starlight/physx/bvh/AABB.hh"
#include "BoundingVolumeComponent.hh"
#include "TransformComponent.hh"

namespace sl {

void MeshComponent::onInit() {
    auto& entity = getEntity();
    if (not entity.has<BoundingVolumeComponent>()) {
        auto aabb = std::make_unique<AABB>();
        aabb->addExtent(data().mesh->getExtent());

        entity.on<TransformComponent>([&](auto& c) {
            auto& transform = c.data();
            aabb->setTransform(transform);
            data().setTransform(transform);
        });

        entity.add<BoundingVolumeComponent>(std::move(aabb));
    }
}

void MeshComponent::Parser::deserialize(Entity& entity, const nlohmann::json& json)
  const {
    const auto& meshName = json["mesh"].get<std::string>();
    entity.add<MeshComponent>(MeshFactory::get().find(meshName));
}

nlohmann::json MeshComponent::Parser::serialize(const MeshComponentData& data
) const {
    nlohmann::json root;
    root["mesh"] = data.mesh->getName();
    return root;
}

std::string MeshComponent::Parser::getComponentName() const {
    return "MeshComponent";
}

}  // namespace sl
