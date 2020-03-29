#pragma once

#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <starlight/geometry/Mesh.h>

namespace starl::platform::model::assimp {

class AssimpMeshProcessor {
public:
    std::shared_ptr<geometry::Mesh> processMesh(aiMesh*, const aiScene*);
};
}