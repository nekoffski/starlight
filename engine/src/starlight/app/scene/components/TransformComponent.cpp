#include "TransformComponent.hh"

#include "starlight/app/scene/Entity.hh"

#include "BoundingVolumeComponent.hh"
#include "MeshComponent.hh"

namespace sl {

void TransformComponent::onInit() {
    auto& entity = getEntity();

    entity.on<BoundingVolumeComponent>([&](auto& c) {
        c.data().boundingVolume->setTransform(data());
    });

    entity.on<MeshComponent>([&](auto& c) { c.data().setTransform(data()); });
}

void TransformComponent::Parser::deserialize(
  Entity& entity, const nlohmann::json& json
) const {
    entity.add<TransformComponent>();
}

std::string TransformComponent::Parser::getComponentName() const {
    return "TransformComponent";
}

nlohmann::json sl::TransformComponent::Parser::serialize(const TransformComponent& c
) const {
    auto& component = c.data();

    nlohmann::json json;
    //     // todo

    return json;
}

}  // namespace sl
