#include "SceneDeserializer.h"

namespace sl {

SceneDeserializer::SceneDeserializer(const FileSystem& fileSystem) :
    m_fileSystem(fileSystem) {}

Expected<Scene> SceneDeserializer::deserialize(const std::string& path) const {
    if (not m_fileSystem.isFile(path))
        return Unexpected{ "Scene file doesn't exist" };

    try {
        const auto root = kc::json::loadJson(m_fileSystem.readFile(path));
        return parseScene(root);
    } catch (kc::json::JsonError& e) {
        return Unexpected{ e.getDetails() };
    }
}

Expected<Scene> SceneDeserializer::parseScene(const kc::json::Node& root) const {
    if (root.isMember("entities")) {
        for (const auto& entity : root["entities"]) {
        }
    }

    if (root.isMember("resources")) {
        for (const auto& resource : root["resources"]) {
        }
    }
}

}  // namespace sl
