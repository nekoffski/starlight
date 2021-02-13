#include "Deserializer.h"

#include "sl/asset/Asset.h"
#include "sl/asset/AssetLoader.hpp"
#include "sl/core/Json.h"
#include "sl/core/error/Errors.hpp"
#include "sl/graphics/Cubemap.h"
#include "sl/utils/Globals.h"

using namespace sl::core::error;

namespace sl::application {

Deserializer::Deserializer(const std::string& path, std::shared_ptr<core::FileSystem> fileSystem)
    : m_path(path)
    , m_fileSystem(fileSystem) {
}

void Deserializer::deserialize(asset::AssetManager& assetManager, std::shared_ptr<scene::Scene> scene) {
    if (not m_fileSystem->isFile(m_path))
        throw DeserializationError{ ErrorCode::FileDoesNotExist, "Could not find file: " + m_path };

    auto fileContent = m_fileSystem->readFile(m_path);

    try {
        auto json = core::parseJson(fileContent);

        if (not json.isMember("assets") || not json.isMember("scene"))
            throw DeserializationError{ ErrorCode::ProjectJsonIsInvalid };

        deserializeAssets(assetManager, json["assets"]);
        deserializeScene(scene, assetManager, json["scene"]);

    } catch (JsonError& e) {
        throw DeserializationError{ ErrorCode::InvalidJsonString, e.getDetails() };
    } catch (Json::Exception& e) {
        throw DeserializationError{ ErrorCode::ProjectJsonIsInvalid };
    }
}

void Deserializer::deserializeAssets(asset::AssetManager& assetManager, Json::Value& assetsJson) {
    for (auto& asset : assetsJson) {
        auto assetType = asset["type"].asInt();
        if (assetType > asset::AssetTypeMax) {
        }

        auto name = asset["name"].asString();
        auto id = asset["id"].asUInt();

        using asset::AssetType;

        switch (static_cast<AssetType>(assetType)) {
        case AssetType::shader: {
            auto& paths = asset["paths"];
            auto shader = asset::AssetLoader::loadGlobalPath<graphics::Shader>(
                paths[0].asString(), paths[1].asString(), paths[2].asString());
            shader->setId(id);
            auto shaderAsset = std::make_shared<asset::ShaderAsset>(shader, name);
            assetManager.addAsset(shaderAsset);

            break;
        }

        case AssetType::model: {
            auto path = asset["paths"][0].asString();
            auto model = asset::AssetLoader::loadGlobalPath<geometry::Model>(path);
            model->setId(id);
            auto modelAsset = std::make_shared<asset::ModelAsset>(model, name);
            assetManager.addAsset(modelAsset);

            break;
        }

        case AssetType::cubemap: {
            auto& paths = asset["paths"];

            graphics::CubemapArgs faces;
            for (int i = 0; i < faces.size(); ++i)
                faces[i] = paths[i].asString();

            auto cubemap = asset::AssetLoader::loadGlobalPath<graphics::Cubemap>(faces);
            cubemap->setId(id);
            auto cubemapAsset = std::make_shared<asset::CubemapAsset>(cubemap, name);
            assetManager.addAsset(cubemapAsset);

            break;
        }
        }
    }
}

void Deserializer::deserializeScene(std::shared_ptr<scene::Scene> scene, asset::AssetManager& assetManager, Json::Value& sceneJson) {
    if (sceneJson.isMember("skybox-id")) {
        auto skyboxId = sceneJson["skybox-id"].asUInt();
        auto cubemapAsset = assetManager.getAssetById(skyboxId)->as<asset::CubemapAsset>();
        auto skybox = scene::Skybox::create(utils::Globals::shaders->defaultCubemapShader, cubemapAsset->cubemap);
        scene->skybox = skybox;
    }

    auto& entities = sceneJson["entities"];
}
}
