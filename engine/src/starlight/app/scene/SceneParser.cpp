#include "SceneParser.hh"

#include "starlight/core/Time.hh"
#include "starlight/app/factories/SkyboxFactory.hh"

#include "Components.hh"

namespace sl {

SceneParser::SceneParser(const kstd::FileSystem* fs)
    : m_fs(fs) {
    registerParser<DirectionalLightComponent, DirectionalLightComponent::Parser>();
    registerParser<PointLightComponent, PointLightComponent::Parser>();
    registerParser<TransformComponent, TransformComponent::Parser>();
    registerParser<MeshRendererComponent, MeshRendererComponent::Parser>();
}

void SceneParser::serialize(Scene& scene, const std::string& path) {
    log::info("Serializing scene: {}", path);
    nlohmann::json root;

    root["ts"] = getTimeString("%Y-%m-%d %H:%M:%S");

    if (auto skybox = scene.getSkybox(); skybox) {
        const auto skyboxName = skybox->getName();
        log::debug("Saving skybox: {}", skyboxName);
        root["skybox"] = skyboxName;
    }

    scene.forEach([&](Entity& entity) {
        root["entities"].push_back(serializeEntity(entity));
    });

    const auto buffer = root.dump();
    log::debug("Parsed scene: {}", buffer);

    m_fs->writeFile(path, buffer, kstd::FileSystem::WritePolicy::override);
    log::info("Scene successfully saved to: {}", path);
}

nlohmann::json SceneParser::serializeEntity(Entity& entity) {
    nlohmann::json node;
    log::debug("Processing entity: {}", entity.getName());
    node["name"] = entity.getName();

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

    return node;
}

kstd::SharedPtr<Scene> SceneParser::deserialize(const std::string& path) {
    auto scene = kstd::makeShared<Scene>();

    log::info("Deserializing scene: {}", path);
    log::expect(m_fs->isFile(path), "Scene file does not exist");
    auto root = nlohmann::json::parse(m_fs->readFile(path));

    if (json::hasField(root, "skybox")) {
        const auto skybox = root["skybox"].get<std::string>();
        log::debug("Found skybox: {}", skybox);
        scene->setSkybox(SkyboxFactory::get().load(skybox));
    }

    log::debug("Processing entitites");
    for (const auto& entityNode : root["entities"])
        deserializeEntity(*scene, entityNode);

    log::info("Scene successfully loaded: {}", path);

    return scene;
}

void SceneParser::deserializeEntity(Scene& scene, const nlohmann::json& node) {
    const auto entityName = node["name"].get<std::string>();
    log::debug("Processing entity: {}", entityName);

    auto& entity           = scene.addEntity(entityName);
    const auto& components = node["components"];

    for (const auto& [componentName, body] :
         components.get<nlohmann::json::object_t>()) {
        log::debug("Processing component: {}", componentName);

        if (not deserializeComponent(componentName, entity, body))
            log::error("Could not find deserializer for: {}", componentName);
    }
}

bool SceneParser::deserializeComponent(
  const std::string& name, Entity& entity, const nlohmann::json& node
) {
    const auto parser = m_componentParsers.find(name);
    if (parser == m_componentParsers.end()) return false;

    parser->second->deserialize(entity, node);
    return true;
}

}  // namespace sl
