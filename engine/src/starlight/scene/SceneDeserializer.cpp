#include "SceneDeserializer.hh"

#include "starlight/core/Log.hh"

#include "components/All.hh"

namespace sl {

SceneDeserializer::SceneDeserializer(const FileSystem& fileSystem) :
    m_fileSystem(fileSystem) {
    registerDeserializer("MaterialComponent", MaterialComponent::deserialize);
    registerDeserializer("TransformComponent", TransformComponent::deserialize);
    registerDeserializer("PointLightComponent", PointLightComponent::deserialize);
    registerDeserializer("MeshComponent", MeshComponent::deserialize);
}

void SceneDeserializer::registerDeserializer(
  const std::string& componentName, Callback&& callback
) {
    ASSERT(
      not m_componentDeserializers.contains(componentName),
      "Deserializer for '{}' already registered", componentName
    );
    m_componentDeserializers[componentName] = std::move(callback);
}

void SceneDeserializer::deserialize(Scene& scene, const std::string& path) const {
    // TODO: handle errors gratefully instead of crashing
    ASSERT(m_fileSystem.isFile(path), "Scene file doesn't exist");
    try {
        const auto root = kc::json::loadJson(m_fileSystem.readFile(path));
        parseScene(scene, root);
    } catch (kc::json::JsonError& e) {
        FATAL_ERROR("Could not parse scene file: {}", e.getDetails());
    }
}

void SceneDeserializer::parseScene(Scene& scene, const kc::json::Node& root) const {
    scene.clear();

    if (root.isMember("entities")) {
        for (const auto& entityDefinition : root["entities"]) {
            ASSERT(
              entityDefinition.isMember("name"),
              "Could not process entity, 'name' field not found"
            );

            const auto name = getField<std::string>(entityDefinition, "name");
            auto entity     = scene.addEntity(name);

            for (auto& componentDefinition : entityDefinition["components"]) {
                ASSERT(
                  componentDefinition.isMember("component"),
                  "Could not process entity, 'component' field not found"
                );
                const auto componentName =
                  getField<std::string>(componentDefinition, "component");

                if (const auto deserializer =
                      m_componentDeserializers.find(componentName);
                    deserializer != m_componentDeserializers.end()) {
                    LOG_TRACE("Deserializing '{}'", componentName);
                    deserializer->second(entity, componentDefinition);
                } else {
                    LOG_WARN(
                      "Could not find deserializer for '{}', scene config file is either "
                      "corrupted or the component hasn't been registered",
                      componentName
                    );
                }
            }
        }
    }

    if (root.isMember("resources")) {
        for (const auto& resource : root["resources"]) {
        }
    }
}

}  // namespace sl
