#include "Serializer.h"

#include <unordered_set>

#include "sl/core/Logger.h"
#include "sl/ecs/Entity.h"
#include "sl/utils/Globals.h"

namespace sl::app {

Serializer::Serializer(const std::string& path, std::shared_ptr<core::FileSystem> fileSystem)
    : m_path(path)
    , m_fileSystem(fileSystem) {
}

void Serializer::serialize(asset::AssetManager& assetManager, std::shared_ptr<scene::Scene> scene) {
    auto filePath = m_path;

    if (not filePath.ends_with(extension))
        filePath += extension;

    serializeAssets(assetManager);
    serializeScene(scene);

    m_fileSystem->writeFile(filePath, m_jsonBuilder.asString());
}

void Serializer::serializeAssets(asset::AssetManager& assetManager) {
    m_jsonBuilder
        .beginObject("assets")
        .beginArray("cubemaps");

    for (auto& [name, cubemap] : assetManager.getCubemaps().getAll()) {
        auto faces = cubemap->getFaces();
        m_jsonBuilder
            .beginObject()
            .addField("name", name)
            .addField("id", cubemap->getId())
            .addField("paths",
                std::vector<std::string> { faces.begin(), faces.end() })
            .endObject();
    }

    std::unordered_set<std::string> modelsToLoad;

    m_jsonBuilder
        .endArray()
        .beginObject("models")
        .beginArray("meshes");

    for (auto& [name, mesh] : assetManager.getMeshes().getAll()) {
        m_jsonBuilder
            .beginObject()
            .addField("name", name)
            .addField("id", mesh->getId());

        if (mesh->providedBy.has_value()) {
            auto providedBy = *mesh->providedBy;

            modelsToLoad.insert(providedBy);
            m_jsonBuilder.addField("provided-by", providedBy);
        }

        m_jsonBuilder.endObject();
    }

    m_jsonBuilder
        .endArray()
        .addField("paths", modelsToLoad)
        .endObject()
        .beginObject("default-assets")
        .beginArray("shaders");

    if (GLOBALS().shaders)
        for (auto& [name, shader] : GLOBALS().shaders->shadersByName)
            m_jsonBuilder
                .beginObject()
                .addField("name", name)
                .addField("id", shader->getId())
                .endObject();

    m_jsonBuilder
        .endArray()
        .beginArray("meshes");

    if (GLOBALS().geom)
        for (auto& [name, mesh] : GLOBALS().geom->meshes)
            m_jsonBuilder
                .beginObject()
                .addField("name", name)
                .addField("id", mesh->getId())
                .endObject();

    m_jsonBuilder
        .endArray()
        .endObject()
        .endObject();
}

void Serializer::serializeScene(std::shared_ptr<scene::Scene> scene) {
    m_jsonBuilder.beginObject("scene").beginArray("entities");

    for (auto& [entityId, entity] : scene->ecsRegistry.getEntities()) {
        m_jsonBuilder.beginObject();
        m_jsonBuilder.addField("id", entityId).addField("name", entity->getName());
        m_jsonBuilder.beginArray("components");

        for (auto& componentIndex : entity->getComponentsIndexes()) {
            m_jsonBuilder.beginObject();
            m_componentsSerializer.serializeComponent(
                componentIndex, m_jsonBuilder, entity->getComponent(componentIndex));
            m_jsonBuilder.endObject();
        }

        m_jsonBuilder.endArray();
        m_jsonBuilder.endObject();
    }
    m_jsonBuilder.endArray();

    if (scene->skybox != nullptr)
        m_jsonBuilder.addField("skybox-id", scene->skybox->cubemap->getId());

    m_jsonBuilder.endObject();
}
}
