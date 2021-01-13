#pragma once

#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "sl/geometry/Mesh.h"

namespace sl::graphics {
class Texture;
}

namespace sl::platform::model {

class AssimpMeshProcessor {
public:
    std::shared_ptr<geometry::Mesh> processMesh(aiMesh*, const aiScene*, const std::string&);

private:
    void initVertexArray(std::shared_ptr<geometry::Mesh>&);
    std::vector<std::shared_ptr<sl::graphics::Texture>> loadTextures(aiMaterial*, const std::string&);
    std::vector<geometry::Vertex> loadVertices(aiMesh*);
    std::vector<unsigned> loadIndices(aiMesh*);
    std::vector<std::shared_ptr<sl::graphics::Texture>> loadMaterialTextures(aiMaterial*, aiTextureType, const std::string&, const std::string&);
};
}
