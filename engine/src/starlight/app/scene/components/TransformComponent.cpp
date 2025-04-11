#include "TransformComponent.hh"

#include "starlight/app/scene/Entity.hh"

namespace sl {

void TransformComponent::onInit() {
    // if (auto& entity = getEntity(); entity.has<ModelComponent>()) {
    //     auto& parent = data();
    //     entity.get<ModelComponent>()->data().getTransform().setParent(parent);
    // }
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
