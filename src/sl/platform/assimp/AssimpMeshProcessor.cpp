#include "AssimpMeshProcessor.h"

#include <kc/core/Log.h>

#include "sl/geom/Mesh.h"
#include "sl/gfx/ElementBuffer.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/TextureManager.h"
#include "sl/gfx/VertexArray.h"
#include "sl/gfx/VertexBuffer.h"

namespace sl::platform::assimp {

using geom::Vertex;

std::shared_ptr<geom::Mesh> AssimpMeshProcessor::processMesh(
    aiMesh* assimpMesh, const aiScene* scene, const std::string& directory
) {
    auto mesh = std::make_shared<geom::Mesh>();

    mesh->vertices = loadVertices(assimpMesh);
    mesh->indices  = loadIndices(assimpMesh);
    mesh->textures = loadTextures(scene->mMaterials[assimpMesh->mMaterialIndex], directory);
    initVertexArray(mesh);

    return mesh;
}

void AssimpMeshProcessor::initVertexArray(std::shared_ptr<geom::Mesh>& mesh) {
    mesh->buildVertexArray();
}

std::vector<std::shared_ptr<sl::gfx::Texture>> AssimpMeshProcessor::loadTextures(
    aiMaterial* material, const std::string& directory
) {
    std::vector<std::shared_ptr<sl::gfx::Texture>> textures;

    auto diffuseMaps =
        loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
    auto specularMaps =
        loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", directory);
    auto normalMaps =
        loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal", directory);
    auto heightMaps =
        loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height", directory);

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return textures;
}

std::vector<unsigned> AssimpMeshProcessor::loadIndices(aiMesh* assimpMesh) {
    std::vector<unsigned> indices;

    for (unsigned i = 0; i < assimpMesh->mNumFaces; ++i) {
        aiFace face = assimpMesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; ++j) indices.push_back(face.mIndices[j]);
    }
    return indices;
}

std::vector<geom::Vertex> AssimpMeshProcessor::loadVertices(aiMesh* assimpMesh) {
    std::vector<Vertex> vertices;

    for (unsigned i = 0; i < assimpMesh->mNumVertices; ++i) {
        Vertex vertex;

        vertex.position = {
            assimpMesh->mVertices[i].x, assimpMesh->mVertices[i].y, assimpMesh->mVertices[i].z};

        vertex.normal = {
            assimpMesh->mNormals[i].x, assimpMesh->mNormals[i].y, assimpMesh->mNormals[i].z};

        if (auto texCoords = assimpMesh->mTextureCoords[0]; texCoords) {
            vertex.textureCoordinates = {texCoords[i].x, texCoords[i].y};
        } else {
            vertex.textureCoordinates = {0.0f, 0.0f};
        }

        vertex.tangent = {
            assimpMesh->mTangents[i].x, assimpMesh->mTangents[i].y, assimpMesh->mTangents[i].z};

        vertex.bitangent = {
            assimpMesh->mBitangents[i].x, assimpMesh->mBitangents[i].y,
            assimpMesh->mBitangents[i].z};

        vertices.push_back(std::move(vertex));
    }
    return vertices;
}

std::vector<std::shared_ptr<sl::gfx::Texture>> AssimpMeshProcessor::loadMaterialTextures(
    aiMaterial* material, aiTextureType textureType, const std::string& typeName,
    const std::string& directory
) {
    std::vector<std::shared_ptr<sl::gfx::Texture>> textures;
    for (unsigned i = 0; i < material->GetTextureCount(textureType); ++i) {
        aiString str;
        material->GetTexture(textureType, i, &str);

        // TODO: OPTIMIZE, store texture in models as most of mesh reuse them!
        auto& textureManager = gfx::TextureManager::get();
        textures.push_back(
            textureManager.createTexture().fromPath(directory + "/" + str.C_Str()).get()
        );
    }
    return textures;
}
}  // namespace sl::platform::assimp
