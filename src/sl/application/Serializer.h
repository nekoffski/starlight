#pragma once

#include "sl/asset/AssetManager.h"
#include "sl/core/FileSystem.h"
#include "sl/core/Json.h"
#include "sl/core/Logger.h"
#include "sl/scene/Scene.h"

namespace sl::application {

class Serializer {
public:
    explicit Serializer(const std::string& path, const std::string& filename)
        : m_path(path)
        , m_filename(filename) {
    }

    void serialize(asset::AssetManager& assetManager, std::shared_ptr<scene::Scene> scene) {
        const std::string extension = ".starscene";
        auto filePath = m_path + "/" + m_filename + extension;

        serializeAssets(assetManager);
        serializeScene(scene);

        core::FileSystem::writeFile(filePath, m_jsonBuilder.asString());
    }

private:
    void serializeAssets(asset::AssetManager& assetManager) {
        m_jsonBuilder.beginArray("assets");

        for (auto& [assetType, assetMap] : assetManager.getAllAssets()) {
            auto iAssetType = static_cast<int>(assetType);
            for (auto& [assetName, asset] : assetMap) {
                m_jsonBuilder.beginObject();
                m_jsonBuilder.addField("name", assetName).addField("type", iAssetType).addField("id", asset->id);
                m_jsonBuilder.endObject();
            }
        }

        m_jsonBuilder.endArray();
    }

    void serializeScene(std::shared_ptr<scene::Scene> scene) {
        m_jsonBuilder.beginObject("scene").beginArray("entities");

        for (auto& [entityId, entity] : scene->ecsRegistry.getEntities()) {
            m_jsonBuilder.beginObject();
            m_jsonBuilder.addField("id", entityId).addField("name", entity->getName());
            m_jsonBuilder.beginArray("components");

            m_jsonBuilder.endArray();
            m_jsonBuilder.endObject();
        }

        m_jsonBuilder.endArray();
        m_jsonBuilder.endObject();
    }

    core::JsonBuilder m_jsonBuilder;

    std::string m_path;
    std::string m_filename;
};
}
