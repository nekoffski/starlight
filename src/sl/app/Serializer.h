#pragma once

#include <kc/core/FileSystem.h>
#include <kc/json/Json.h>

#include "ComponentsSerializer.h"
#include "sl/asset/AssetManager.h"
#include "sl/scene/Scene.h"

namespace sl::app {

class Serializer {
public:
    explicit Serializer(const std::string& path);

    void serialize(asset::AssetManager& assetManager, scene::Scene* scene,
        const kc::core::FileSystem& fileSystem = kc::core::FileSystem {});

    inline static const std::string extension = ".starscene";

private:
    void serializeAssets(asset::AssetManager& assetManager);
    void serializeScene(scene::Scene* scene);

    void serializeDefaultAssets();

    kc::json::JsonBuilder m_jsonBuilder;

    std::string m_path;

    ComponentsSerializer m_componentsSerializer;
};
}
