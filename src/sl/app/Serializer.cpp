#include "Serializer.h"

#include <unordered_set>

#include <kc/core/Log.h>

#include "sl/ecs/Entity.h"
#include "sl/glob/Globals.h"

namespace sl::app {

Serializer::Serializer(const std::string& path)
    : m_path(path) {
}

void Serializer::serialize(asset::AssetManager& assetManager, scene::Scene* scene,
    const kc::core::FileSystem& fileSystem) {

    auto filePath = m_path;

    if (not filePath.ends_with(extension))
        filePath += extension;

    serializeAssets(assetManager);
    serializeScene(scene);

    fileSystem.writeFile(filePath, m_jsonBuilder.asString());
}

void Serializer::serializeAssets(asset::AssetManager& assetManager) {
    m_jsonBuilder
        .beginObject("assets")
        .beginArray("cubemaps");

    for (auto& [name, cubemap] : assetManager.getCubemaps().getAll()) {
        auto faces = cubemap->getFacesPaths();
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
        .endObject();

    m_jsonBuilder.beginArray("textures");

    for (auto& [name, texture] : assetManager.getTextures().getAll())
        m_jsonBuilder
            .beginObject()
            .addField("name", name)
            .addField("id", texture->getId())
            .addField("path", texture->path)
            .endObject();

    m_jsonBuilder.endArray();

    serializeDefaultAssets();

    m_jsonBuilder.endObject();
}

void Serializer::serializeDefaultAssets() {

    m_jsonBuilder
        .beginObject("default-assets")
        .beginArray("shaders");

    if (glob::Globals::get().shaders)
        for (auto& [name, shader] : glob::Globals::get().shaders->shadersByName)
            m_jsonBuilder
                .beginObject()
                .addField("name", name)
                .addField("id", shader->getId())
                .endObject();

    m_jsonBuilder
        .endArray()
        .beginArray("meshes");

    if (glob::Globals::get().geom)
        for (auto& [name, mesh] : glob::Globals::get().geom->meshes)
            m_jsonBuilder
                .beginObject()
                .addField("name", name)
                .addField("id", mesh->getId())
                .endObject();

    m_jsonBuilder
        .endArray()
        .endObject();
}

void Serializer::serializeScene(scene::Scene* scene) {
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

    if (scene->skybox.has_value())
        m_jsonBuilder.addField("skybox-id", scene->skybox->cubemap->getId());

    m_jsonBuilder.endObject();
}
}
