#include "TransformComponent.hh"

#include "starlight/app/scene/Entity.hh"

#include "BoundingVolumeComponent.hh"
#include "MeshRendererComponent.hh"

namespace sl {

void TransformComponent::onInit() {
    auto& entity = getEntity();

    entity.on<BoundingVolumeComponent>([&](auto& c) {
        c.data().boundingVolume->setTransform(data());
    });

    entity.on<MeshRendererComponent>([&](auto& c) { c.data().setTransform(data()); }
    );
}

void TransformComponent::Parser::deserialize(Entity& entity, kstd::BinaryReader& br)
  const {
    entity.add<TransformComponent>();
}

std::string TransformComponent::Parser::getComponentName() const {
    return "TransformComponent";
}

void sl::TransformComponent::Parser::serialize(
  const TransformComponent& c, kstd::BinaryWriter& bw
) const {
    auto& component = c.data();
}

}  // namespace sl
