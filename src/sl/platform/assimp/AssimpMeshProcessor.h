#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <memory>

#include "sl/geom/Mesh.h"

namespace sl::gfx {
class Texture;
}

namespace sl::platform::assimp {

class AssimpMeshProcessor {
   public:
    std::shared_ptr<geom::Mesh> processMesh(aiMesh*, const aiScene*, const std::string&);

   private:
    void initVertexArray(std::shared_ptr<geom::Mesh>&);
    std::vector<std::shared_ptr<sl::gfx::Texture>> loadTextures(aiMaterial*, const std::string&);
    std::vector<geom::Vertex> loadVertices(aiMesh*);
    std::vector<unsigned> loadIndices(aiMesh*);
    std::vector<std::shared_ptr<sl::gfx::Texture>>
        loadMaterialTextures(aiMaterial*, aiTextureType, const std::string&, const std::string&);
};
}  // namespace sl::platform::assimp
