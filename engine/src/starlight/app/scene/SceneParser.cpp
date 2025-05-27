#include "SceneParser.hh"

#include "starlight/core/Time.hh"
#include "starlight/app/factories/SkyboxFactory.hh"

#include "Components.hh"

namespace sl {

static constexpr u32 magicNumber = 0xc4f3b33f;

SceneParser::SceneParser() {
    registerParser<DirectionalLightComponent, DirectionalLightComponent::Parser>();
    registerParser<PointLightComponent, PointLightComponent::Parser>();
    registerParser<TransformComponent, TransformComponent::Parser>();
    registerParser<MeshRendererComponent, MeshRendererComponent::Parser>();
}

void SceneParser::serialize(Scene& scene, const std::string& path) {
    log::info("Serializing scene: {}", path);
    kstd::BinaryWriter bw;

    bw.write(magicNumber);

    if (auto skybox = scene.getSkybox(); skybox) {
        bw.write(skybox->getName());
        log::debug("Skybox: {}", skybox->getName());
    } else {
        log::debug("Scene doesn't have skybox");
        bw.write(std::string{});
    }

    // scene.forEach([&](Entity& entity) {
    //     root["entities"].push_back(serializeEntity(entity));
    // });

    kstd::writeBinaryFile(path, bw.getBuffer());
    log::info("Scene successfully saved to: {}", path);
}

void SceneParser::serializeEntity(Entity& entity) {
    // nlohmann::json node;
    // log::debug("Processing entity: {}", entity.getName());
    // node["name"] = entity.getName();

    for (const auto component : entity.getComponentTypes()) {
        log::debug("Processing component: {}", component.name());
        // if (auto it = m_serializers.find(component); it != m_serializers.end()) {
        //     auto& serializer = it->second;

        //     log::expect(
        //       false, "fixme - serializer should push into the existing object"
        //     );
        //     // node["components"][serializer] =
        //     //   serializer->serialize(entity.getComponent(component));
        // } else {
        //     log::error("Serializer not found: {}", component.name());
        // }
    }

    // return node;
}

kstd::SharedPtr<Scene> SceneParser::deserialize(const std::string& path) {
    auto scene = kstd::makeShared<Scene>();

    log::info("Deserializing scene: {}", path);
    log::expect(kstd::isFile(path), "Scene file does not exist");

    kstd::BinaryReader br{ kstd::readBinaryFile(path) };

    const auto magic = br.read<u32>();
    log::expect(magic == magicNumber, "Invalid scene file, invalid opening byte");

    log::debug("Scene file looks correct, trying to parse...");

    if (auto skyboxName = br.read<std::string>(); skyboxName.empty()) {
        log::debug("No skybox specified");
    } else {
        log::debug("Skybox name: '{}'", skyboxName);
        scene->setSkybox(SkyboxFactory::get().load(skyboxName));
    }

    // log::debug("Processing entitites");
    // for (const auto& entityNode : root["entities"])
    //     deserializeEntity(*scene, entityNode);

    log::info("Scene successfully loaded: {}", path);
    return scene;
}

void SceneParser::deserializeEntity(Scene& scene, const nlohmann::json& node) {
    // const auto entityName = node["name"].get<std::string>();
    // log::debug("Processing entity: {}", entityName);

    // auto& entity           = scene.addEntity(entityName);
    // const auto& components = node["components"];

    // for (const auto& [componentName, body] :
    //      components.get<nlohmann::json::object_t>()) {
    //     log::debug("Processing component: {}", componentName);

    //     if (not deserializeComponent(componentName, entity, body))
    //         log::error("Could not find deserializer for: {}", componentName);
    // }
}

bool SceneParser::deserializeComponent(
  const std::string& name, Entity& entity, const nlohmann::json& node
) {
    // const auto parser = m_componentParsers.find(name);
    // if (parser == m_componentParsers.end()) return false;

    // parser->second->deserialize(entity, node);
    // return true;
}

}  // namespace sl
