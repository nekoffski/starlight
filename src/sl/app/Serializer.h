#pragma once

#include <kc/json/Json.h>

#include "ComponentsSerializer.h"
#include "sl/asset/AssetManager.h"
#include "sl/core/FileSystem.h"
#include "sl/scene/Scene.h"

namespace sl::app {

class Serializer {
public:
    explicit Serializer(const std::string& path,
        std::shared_ptr<core::FileSystem> fileSystem = std::make_shared<core::FileSystem>());

    void serialize(asset::AssetManager& assetManager, std::shared_ptr<scene::Scene> scene);

    inline static const std::string extension = ".starscene";

private:
    void serializeAssets(asset::AssetManager& assetManager);
    void serializeScene(std::shared_ptr<scene::Scene> scene);

    void serializeDefaultAssets();

    kc::json::JsonBuilder m_jsonBuilder;

    std::string m_path;

    std::shared_ptr<core::FileSystem> m_fileSystem;

    ComponentsSerializer m_componentsSerializer;
};
}
