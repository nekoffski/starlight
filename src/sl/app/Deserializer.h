#pragma once

#include <memory>
#include <string>

#include <kc/json/Json.h>

#include "ComponentsDeserializer.h"
#include "sl/asset/AssetManager.h"
#include "sl/core/FileSystem.h"
#include "sl/scene/Scene.h"

namespace sl::app {

class Deserializer {
public:
    explicit Deserializer(asset::AssetManager& assetManager, std::shared_ptr<scene::Scene> scene);

    void deserialize(const std::string& path, std::shared_ptr<core::FileSystem> fileSystem = std::make_shared<core::FileSystem>());

private:
    void deserializeAssets(kc::json::Node&);
    void deserializeScene(kc::json::Node&);

    void deserializeDefaultAssets(kc::json::Node&);

    asset::AssetManager& m_assetManager;
    std::shared_ptr<scene::Scene> m_scene;

    ComponentsDeserializer m_componentsDeserializer;

    std::unordered_map<std::string, std::string> m_assetsIdRedirections;
};
}
