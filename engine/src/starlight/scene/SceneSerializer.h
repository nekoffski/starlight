#pragma once

#include <string>

#include "starlight/core/utils/FileSystem.h"
#include "starlight/core/utils/Json.hpp"

#include "Scene.h"

namespace sl {

class SceneSerializer {
public:
    explicit SceneSerializer(const FileSystem& fileSystem);

    void serialize(Scene& scene, const std::string& outputPath) const;

private:
    std::string parseScene(Scene& scene) const;
    void save(const std::string& buffer, const std::string path) const;

    kc::json::Node processEntities(Scene& scene) const;
    kc::json::Node processResources(Scene& scene) const;

    const FileSystem& m_fileSystem;
};

}  // namespace sl
