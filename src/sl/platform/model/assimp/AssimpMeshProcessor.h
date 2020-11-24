#pragma once

#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "sl/geometry/Mesh.h"

namespace sl::platform::texture {
class Texture;
}

namespace sl::platform::model::assimp {

class AssimpMeshProcessor {
public:
    std::shared_ptr<geometry::Mesh> processMesh(aiMesh*, const aiScene*, std::string);

private:
    void initVertexArray(std::shared_ptr<geometry::Mesh>&);
    std::vector<std::shared_ptr<texture::Texture>> loadTextures(aiMaterial*, std::string);
    std::vector<geometry::Vertex> loadVertices(aiMesh*);
    std::vector<unsigned> loadIndices(aiMesh*);
    std::vector<std::shared_ptr<texture::Texture>> loadMaterialTextures(aiMaterial*, aiTextureType, std::string, std::string);
};
}
