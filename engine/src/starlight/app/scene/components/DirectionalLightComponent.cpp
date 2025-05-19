#include "DirectionalLightComponent.hh"

#include "starlight/math/Json.hh"

namespace sl {

void DirectionalLightComponent::Parser::deserialize(
  Entity& entity, const nlohmann::json& json
) const {
    const auto color     = json.at("color").get<Vec4<f32>>();
    const auto direction = json.at("direction").get<Vec3<f32>>();

    log::debug("Deserialized DirectionalLightComponent: {}/{}", color, direction);

    entity.add<DirectionalLightComponent>(color, direction);
}

std::string DirectionalLightComponent::Parser::getComponentName() const {
    return "DirectionalLightComponent";
}

nlohmann::json sl::DirectionalLightComponent::Parser::serialize(
  const DirectionalLightComponent& c
) const {
    auto& component = c.data();

    nlohmann::json json;
    json["color"]     = component.color;
    json["direction"] = component.direction;

    return json;
}

}  // namespace sl
