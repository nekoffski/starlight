#include "AssimpModelLoaderImpl.h"

#include "AssimpMeshProcessor.h"
#include "sl/core/Logger.h"
#include "sl/core/error/Errors.hpp"
#include "sl/geometry/Model.h"

namespace sl::platform::model {

std::shared_ptr<geometry::Model> AssimpModelLoaderImpl::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

    auto model = std::make_shared<geometry::Model>();
    model->path = path;
    model->directory = path.substr(0, path.find_last_of("/"));

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        SL_ERROR("could not load scene");
        throw core::error::ModelError{ core::error::ErrorCode::CouldNotLoadModel };
    }

    AssimpMeshProcessor meshProcessor;
    processNode(scene->mRootNode, scene, meshProcessor, model);

    return model;
}

void AssimpModelLoaderImpl::processNode(aiNode* node, const aiScene* scene, AssimpMeshProcessor& meshProcessor, std::shared_ptr<geometry::Model>& model) {
    for (int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes.push_back(meshProcessor.processMesh(mesh, scene, model->directory));
    }

    for (int i = 0; i < node->mNumChildren; ++i)
        processNode(node->mChildren[i], scene, meshProcessor, model);
}
}
