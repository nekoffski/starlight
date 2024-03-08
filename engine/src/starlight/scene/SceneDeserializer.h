#pragma once

#include <string>

#include "starlight/core/Core.h"
#include "starlight/core/utils/FileSystem.h"
#include "starlight/core/utils/Json.hpp"

#include "Scene.h"

namespace sl {

class SceneDeserializer {
public:
    explicit SceneDeserializer(const FileSystem& fileSystem);

    Expected<Scene> deserialize(const std::string& path) const;

private:
    Expected<Scene> parseScene(const kc::json::Node& root) const;

    const FileSystem& m_fileSystem;
};

}  // namespace sl
