#include "BuiltinSerializers.hh"

#include "starlight/core/Json.hh"
#include "starlight/app/factories/MaterialFactory.hh"
#include "starlight/app/factories/MeshFactory.hh"
#include "starlight/app/factories/ModelFactory.hh"

#include "Components.hh"

#include "SceneParser.hh"

namespace sl {

nlohmann::json serializeDirectionalLight(DirectionalLightComponent& c) {
    auto& component = c.data();

    nlohmann::json json;
    json["color"]     = component.color;
    json["direction"] = component.direction;

    return json;
}

void deserializeDirectionalLight(Entity& entity, const nlohmann::json& json) {
    const auto color     = json.at("color").get<Vec4<f32>>();
    const auto direction = json.at("direction").get<Vec3<f32>>();

    log::debug("Deserialized DirectionalLightComponent: {}/{}", color, direction);

    entity.add<DirectionalLightComponent>(color, direction);
}

nlohmann::json serializePointLight(PointLightComponent& c) {
    auto& component = c.data();

    nlohmann::json json;
    const auto& pointLightData = component.getShaderData();

    json["color"]       = pointLightData.color;
    json["position"]    = pointLightData.position;
    json["attenuation"] = pointLightData.attenuation;

    return json;
}

void deserializePointLight(Entity& entity, const nlohmann::json& json) {
    const auto color       = json.at("color").get<Vec4<f32>>();
    const auto position    = json.at("position").get<Vec3<f32>>();
    const auto attenuation = json.at("attenuation").get<Vec3<f32>>();

    log::debug(
      "Deserialized PointLightComponent: {}/{}/{}", color, position, attenuation
    );
    entity.add<PointLightComponent>(color, position, attenuation);
}

nlohmann::json serializeModel(ModelComponent& c) {
    auto& component = c.data();

    nlohmann::json json;
    // todo

    return json;
}

void deserializeModel(Entity& entity, const nlohmann::json& json) {
    auto& meshFactory     = MeshFactory::get();
    auto& modelFactory    = ModelFactory::get();
    auto& materialFactory = MaterialFactory::get();

    auto model = [&] {
        switch (const auto type = json.at("type").get<Model::Type>(); type) {
            case Model::Type::custom: {
                log::debug("Deserializing CustomModel");
                auto model = modelFactory.create();
                for (const auto& sub : json.at("body")) {
                    const auto meshName     = sub.at("mesh").get<std::string>();
                    const auto materialName = sub.at("material").get<std::string>();

                    log::debug("SubModel: {}/{}", meshName, materialName);

                    model->addSub(
                      meshFactory.find(meshName), materialFactory.load(materialName)
                    );
                }
                return model;
            }

            case Model::Type::wavefront: {
                break;
            }
        }
    }();
    entity.add<ModelComponent>(model);
}

nlohmann::json serializeTransform(TransformComponent& c) {
    auto& component = c.data();

    nlohmann::json json;
    // todo

    return json;
}

void deserializeTransform(Entity& entity, const nlohmann::json& json) {
    entity.add<TransformComponent>();
}

void registerBuiltinComponents(SceneParser& parser) {
    parser
      .registerComponent<DirectionalLightComponent>(
        "DirectionalLightComponent", serializeDirectionalLight,
        deserializeDirectionalLight
      )
      .registerComponent<PointLightComponent>(
        "PointLightComponent", serializePointLight, deserializePointLight
      )
      .registerComponent<ModelComponent>(
        "ModelComponent", serializeModel, deserializeModel
      )
      .registerComponent<TransformComponent>(
        "TransformComponent", serializeTransform, deserializeTransform
      );
}

}  // namespace sl
