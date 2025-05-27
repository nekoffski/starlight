#include "PointLightComponent.hh"

#include "starlight/math/Json.hh"

namespace sl {

void PointLightComponent::Parser::deserialize(Entity& entity, kstd::BinaryReader& br)
  const {
    // const auto color       = json.at("color").get<Vec4<f32>>();
    // const auto position    = json.at("position").get<Vec3<f32>>();
    // const auto attenuation = json.at("attenuation").get<Vec3<f32>>();

    // entity.add<PointLightComponent>(color, position, attenuation);
}

std::string PointLightComponent::Parser::getComponentName() const {
    return "PointLightComponent";
}

void sl::PointLightComponent::Parser::serialize(
  const PointLightComponent& c, kstd::BinaryWriter& bw
) const {
    // auto& component = c.data();

    // nlohmann::json json;
    // const auto& pointLightData = component.getShaderData();

    // json["color"]       = pointLightData.color;
    // json["position"]    = pointLightData.position;
    // json["attenuation"] = pointLightData.attenuation;

    // return json;
}

}  // namespace sl
