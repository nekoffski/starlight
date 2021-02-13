#pragma once

#include <memory>
#include <string>

#include "sl/asset/AssetManager.h"
#include "sl/core/FileSystem.h"
#include "sl/core/Json.h"
#include "sl/scene/Scene.h"

namespace sl::application {

class Deserializer {
public:
    explicit Deserializer(const std::string& path, std::shared_ptr<core::FileSystem> fileSystem = std::make_shared<core::FileSystem>());

    void deserialize(asset::AssetManager& assetManager, std::shared_ptr<scene::Scene> scene);

private:
    void deserializeAssets(asset::AssetManager& assetManager, Json::Value&);
    void deserializeScene(std::shared_ptr<scene::Scene> scene, asset::AssetManager& assetManager, Json::Value&);

    std::string m_path;
    std::shared_ptr<core::FileSystem> m_fileSystem;
};
}
