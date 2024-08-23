#pragma once

#include <string>

#include "starlight/core/FileSystem.hh"
#include "starlight/core/Json.hh"

#include "Scene.hh"

namespace sl {

class SceneSerializer {
public:
    explicit SceneSerializer(const FileSystem& fileSystem);

    void serialize(Scene& scene, const std::string& outputPath) const;

private:
    std::string parseScene(Scene& scene) const;
    void save(const std::string& buffer, const std::string& outputPath) const;

    kc::json::Node processEntities(Scene& scene) const;
    kc::json::Node processResources(Scene& scene) const;

    const FileSystem& m_fileSystem;
};

}  // namespace sl
