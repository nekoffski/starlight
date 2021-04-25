#include "AssimpModelLoaderImpl.h"

#include <fmt/core.h>

#include "AssimpMeshProcessor.h"
#include "sl/core/Errors.hpp"
#include "sl/core/Logger.h"
#include "sl/geom/Model.h"

namespace sl::platform::model {

std::string getModelName(const std::string& modelPath) {
    auto nameBegin = modelPath.find_last_of("/") + 1;
    return modelPath.substr(
        nameBegin,
        modelPath.find_last_of(".") - nameBegin);
}

std::shared_ptr<geom::Model> AssimpModelLoaderImpl::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

    auto model = std::make_shared<geom::Model>();
    model->path = path;
    model->directory = path.substr(0, path.find_last_of("/"));

    if (not scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || not scene->mRootNode) {
        SL_ERROR("could not load scene");
        throw core::ModelError { core::ErrorCode::CouldNotLoadModel };
    }

    AssimpMeshProcessor meshProcessor;
    processNode(scene->mRootNode, scene, meshProcessor, model);

    auto modelName = getModelName(path);

    int i = 0;
    for (auto& mesh : model->meshes) {
        mesh->providedBy = path;
        mesh->name = fmt::format("{}_part_{}", modelName, i++);
    }

    return model;
}

void AssimpModelLoaderImpl::processNode(aiNode* node, const aiScene* scene, AssimpMeshProcessor& meshProcessor, std::shared_ptr<geom::Model>& model) {
    for (int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes.push_back(meshProcessor.processMesh(mesh, scene, model->directory));
    }

    for (int i = 0; i < node->mNumChildren; ++i)
        processNode(node->mChildren[i], scene, meshProcessor, model);
}
}
