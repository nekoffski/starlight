#include "BuiltinSerializers.hh"

#include "starlight/core/Json.hh"
#include "starlight/renderer/MeshComposite.hh"
#include "starlight/app/factories/MaterialFactory.hh"
#include "starlight/app/factories/MeshFactory.hh"

#include "SceneParser.hh"

namespace sl {

nlohmann::json serializeDirectionalLight(DirectionalLight& component) {
    nlohmann::json json;
    json["color"]     = component.color;
    json["direction"] = component.direction;

    return json;
}

void deserializeDirectionalLight(Entity& entity, const nlohmann::json& json) {
    entity.addComponent<DirectionalLight>(
      json.at("color").get<Vec4<f32>>(), json.at("direction").get<Vec3<f32>>()
    );
}

nlohmann::json serializePointLight(PointLight& component) {
    nlohmann::json json;
    const auto& pointLightData = component.getShaderData();

    json["color"]       = pointLightData.color;
    json["position"]    = pointLightData.position;
    json["attenuation"] = pointLightData.attenuation;

    return json;
}

void deserializePointLight(Entity& entity, const nlohmann::json& json) {
    entity.addComponent<PointLight>(
      json.at("color").get<Vec4<f32>>(), json.at("position").get<Vec3<f32>>(),
      json.at("attenuation").get<Vec3<f32>>()
    );
}

nlohmann::json serializeMeshComposite(MeshComposite& component) {
    nlohmann::json json;

    // just root for now
    auto& root = component.getRoot();

    json["material"] = root.material->name;
    json["mesh"]     = root.mesh->name;

    return json;
}

void deserializeMeshComposite(Entity& entity, const nlohmann::json& json) {
    // TODO: store default materials/meshes/shaders/textures in some lookup table
    static const auto getMesh = [](const std::string& name) {
        return MeshFactory::get().find(name);
    };

    static const auto getMaterial = [](const std::string& name) {
        return MaterialFactory::get().load(name);
    };

    auto mesh     = getMesh(json.at("mesh").get<std::string>());
    auto material = getMaterial(json.at("material").get<std::string>());

    entity.addComponent<MeshComposite>(mesh, material);
}

void registerBuiltinComponents(SceneParser& parser) {
    parser
      .registerComponent<DirectionalLight>(
        "DirectionalLight", serializeDirectionalLight, deserializeDirectionalLight
      )
      .registerComponent<PointLight>(
        "PointLight", serializePointLight, deserializePointLight
      )
      .registerComponent<MeshComposite>(
        "MeshComposite", serializeMeshComposite, deserializeMeshComposite
      );
}

}  // namespace sl
