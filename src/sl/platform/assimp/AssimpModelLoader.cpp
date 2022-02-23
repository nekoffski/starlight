#include "AssimpModelLoader.h"

#include <kc/core/Log.h>
#include <kc/core/String.h>

#include "AssimpMeshProcessor.h"
#include "sl/core/Errors.hpp"
#include "sl/geom/Model.h"

namespace sl::platform::assimp {

std::string getModelName(const std::string& modelPath) {
    auto nameBegin = modelPath.find_last_of("/") + 1;
    return modelPath.substr(nameBegin, modelPath.find_last_of(".") - nameBegin);
}

std::shared_ptr<geom::Model> AssimpModelLoader::load(const std::string& path) {
    LOG_DEBUG("Loading model: {}", path);

    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                    aiProcess_CalcTangentSpace | aiProcess_GenNormals);

    auto model = std::make_shared<geom::Model>();
    model->path = path;
    model->directory = path.substr(0, path.find_last_of("/"));

    if (not scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || not scene->mRootNode) {
        LOG_ERROR("could not load scene");
        throw core::ModelError{};
    }

    AssimpMeshProcessor meshProcessor;
    processNode(scene->mRootNode, scene, meshProcessor, model);

    using namespace kc::core;

    auto modelName = extractNameFromPath(path, ExtractingMode::withoutExtension);

    int i = 0;

    LOG_DEBUG("Loaded meshes for model: {}", path);

    for (auto& mesh : model->meshes) {
        mesh->providedBy = path;
        mesh->name = fmt::format("{}_part_{}", modelName, i++);

        LOG_DEBUG("{}", mesh->name);
    }

    return model;
}

void AssimpModelLoader::processNode(aiNode* node, const aiScene* scene,
                                    AssimpMeshProcessor& meshProcessor,
                                    std::shared_ptr<geom::Model>& model) {
    for (int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes.push_back(meshProcessor.processMesh(mesh, scene, model->directory));
    }

    for (int i = 0; i < node->mNumChildren; ++i)
        processNode(node->mChildren[i], scene, meshProcessor, model);
}
}  // namespace sl::platform::assimp
