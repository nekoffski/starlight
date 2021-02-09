#pragma once

#include "sl/asset/AssetManager.h"
#include "sl/core/FileSystem.h"
#include "sl/scene/Scene.h"
#include "sl/core/Json.h"

namespace sl::application {

class Serializer {
public:
    explicit Serializer(const std::string& path, const std::string& filename, const core::FileSystem& fileSystem = core::FileSystem{});

    void serialize(asset::AssetManager& assetManager, std::shared_ptr<scene::Scene> scene);

    inline static const std::string extension = ".starscene";

private:
    void serializeAssets(asset::AssetManager& assetManager);
    void serializeScene(std::shared_ptr<scene::Scene> scene);

    core::JsonBuilder m_jsonBuilder;

    std::string m_path;
    std::string m_filename;

    const core::FileSystem& m_fileSystem;
};
}
