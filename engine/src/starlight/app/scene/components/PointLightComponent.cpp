#include "PointLightComponent.hh"

namespace sl {

void PointLightComponent::Parser::deserialize(
  Entity& entity, const nlohmann::json& json
) const {
    const auto color       = json.at("color").get<Vec4<f32>>();
    const auto position    = json.at("position").get<Vec3<f32>>();
    const auto attenuation = json.at("attenuation").get<Vec3<f32>>();

    entity.add<PointLightComponent>(color, position, attenuation);
}

std::string PointLightComponent::Parser::getComponentName() const {
    return "PointLightComponent";
}

nlohmann::json sl::PointLightComponent::Parser::serialize(
  const PointLightComponent& c
) const {
    auto& component = c.data();

    nlohmann::json json;
    const auto& pointLightData = component.getShaderData();

    json["color"]       = pointLightData.color;
    json["position"]    = pointLightData.position;
    json["attenuation"] = pointLightData.attenuation;

    return json;
}

}  // namespace sl
