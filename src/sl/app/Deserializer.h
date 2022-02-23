#pragma once

#include <kc/core/FileSystem.h>
#include <kc/json/Json.h>

#include <memory>
#include <string>

#include "ComponentsDeserializer.h"
#include "sl/asset/AssetManager.h"
#include "sl/scene/Scene.h"

namespace sl::app {

class Deserializer {
   public:
    explicit Deserializer(asset::AssetManager& assetManager, scene::Scene* scene);

    void deserialize(const std::string& path,
                     const kc::core::FileSystem& fileSystem = kc::core::FileSystem{});

   private:
    void deserializeAssets(kc::json::Node&);
    void deserializeScene(kc::json::Node&);

    void deserializeDefaultAssets(kc::json::Node&);

    asset::AssetManager& m_assetManager;
    scene::Scene* m_scene;

    ComponentsDeserializer m_componentsDeserializer;

    std::unordered_map<std::string, std::string> m_assetsIdRedirections;
};
}  // namespace sl::app
