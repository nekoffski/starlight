#include "SceneSerializer.hh"

namespace sl {

SceneSerializer::SceneSerializer(const FileSystem& fileSystem) :
    m_fileSystem(fileSystem) {}

void SceneSerializer::serialize(Scene& scene, const std::string& outputPath) const {
    save(parseScene(scene), outputPath);
}

std::string SceneSerializer::parseScene(Scene& scene) const {
    kc::json::Node root;

    root["entities"]  = processEntities(scene);
    root["resources"] = processResources(scene);

    return kc::json::toString(root);
}

void SceneSerializer::save(const std::string& buffer, const std::string& outputPath)
  const {
    m_fileSystem.writeFile(outputPath, buffer, FileSystem::WritePolicy::override);
}

kc::json::Node SceneSerializer::processEntities(Scene& scene) const {
    kc::json::Node root;

    scene.forEachEntity([&](const std::string& name, Entity* entity) -> void {
        kc::json::Node entityNode;
        entityNode["name"] = name;
        auto& components   = entityNode["components"];

        entity->forEachComponent([&](const Component* component) {
            components.append(component->serialize());
        });

        root.append(entityNode);
    });

    return root;
}

kc::json::Node SceneSerializer::processResources(Scene& scene) const {
    kc::json::Node root;

    return root;
}

}  // namespace sl
