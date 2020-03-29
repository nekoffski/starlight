#include <starlight/platform/model/assimp/AssimpModelLoader.h>

#include <starlight/geometry/Model.h>
#include <starlight/platform/Error.h>
#include <starlight/platform/model/assimp/AssimpMeshProcessor.h>

#include <starlight/core/log/Logger.h>

static auto logger = starl::core::log::createLogger("AssimpModelLoader");

namespace starl::platform::model::assimp {

std::shared_ptr<geometry::Model> AssimpModelLoader::loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

    auto model = std::make_shared<geometry::Model>();

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        logger->error("could not load scene");
        throw PlatformException(ErrorCode::COULD_NOT_LOAD_MODEL, "failed to load assimp scene: " + path);
    }

    AssimpMeshProcessor meshProcessor;
    processNode(scene->mRootNode, scene, meshProcessor, model);

    return model;
}

void AssimpModelLoader::processNode(aiNode* node, const aiScene* scene, AssimpMeshProcessor& meshProcessor, std::shared_ptr<geometry::Model>& model) {
    for (int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes.push_back(meshProcessor.processMesh(mesh, scene));
    }

    for (int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, meshProcessor, model);
    }
}
}
