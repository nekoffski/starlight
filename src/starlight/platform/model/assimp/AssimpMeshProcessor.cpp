#include <starlight/platform/model/assimp/AssimpMeshProcessor.h>

#include <starlight/geometry/Mesh.h>

#include <starlight/platform/gpu/ElementBuffer.h>
#include <starlight/platform/gpu/VertexArray.h>
#include <starlight/platform/gpu/VertexBuffer.h>
#include <starlight/platform/texture/Texture.h>

#include <starlight/core/log/Logger.h>

static auto logger = starl::core::log::createLogger("AssimpMeshProcesor");

namespace starl::platform::model::assimp {

std::shared_ptr<geometry::Mesh> AssimpMeshProcessor::processMesh(aiMesh* assimpMesh, const aiScene* scene, std::string directory) {
    using geometry::Vertex;

    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    std::vector<std::shared_ptr<texture::Texture>> textures;

    auto mesh = std::make_shared<geometry::Mesh>();

    for (unsigned i = 0; i < assimpMesh->mNumVertices; ++i) {
        Vertex vertex;

        vertex.position = {
            assimpMesh->mVertices[i].x,
            assimpMesh->mVertices[i].y,
            assimpMesh->mVertices[i].z
        };

        vertex.normal = {
            assimpMesh->mNormals[i].x,
            assimpMesh->mNormals[i].y,
            assimpMesh->mNormals[i].z
        };

        if (auto texCoords = assimpMesh->mTextureCoords[0]; texCoords) {
            vertex.textureCoordinates = {
                texCoords[i].x, texCoords[i].y
            };
        } else {
            vertex.textureCoordinates = { 0.0f, 0.0f };
        }

        vertex.tangent = {
            assimpMesh->mTangents[i].x,
            assimpMesh->mTangents[i].y,
            assimpMesh->mTangents[i].z
        };

        vertex.bitangent = {
            assimpMesh->mBitangents[i].x,
            assimpMesh->mBitangents[i].y,
            assimpMesh->mBitangents[i].z
        };

        vertices.push_back(std::move(vertex));
    }
    mesh->vertices = std::move(vertices);

    for (unsigned i = 0; i < assimpMesh->mNumFaces; ++i) {
        aiFace face = assimpMesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }
    mesh->indices = std::move(indices);

    aiMaterial* material = scene->mMaterials[assimpMesh->mMaterialIndex];

    auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
    auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", directory);
    auto normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal", directory);
    auto heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height", directory);

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    mesh->textures = std::move(textures);

    auto vao = platform::gpu::VertexArray::create();
    auto vbo = platform::gpu::VertexBuffer::create(&mesh->vertices[0], mesh->vertices.size() * sizeof(Vertex), mesh->vertices.size());
    auto ebo = platform::gpu::ElementBuffer::create(&mesh->indices[0], mesh->indices.size() * sizeof(unsigned), mesh->indices.size());

    // vertices
    vbo->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));
    // normals
    vbo->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));
    // tex coords
    vbo->addMemoryOffsetScheme(2, STARL_FLOAT, sizeof(float));
    // tangents
    vbo->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));
    // bitangents
    vbo->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

    vao->addVertexBuffer(vbo);
    vao->addElementBuffer(ebo);

    mesh->vertexArray = vao;
    return mesh;
}

std::vector<std::shared_ptr<texture::Texture>>
AssimpMeshProcessor::loadMaterialTextures(aiMaterial* material, aiTextureType textureType, std::string typeName, std::string directory) {
    std::vector<std::shared_ptr<texture::Texture>> textures;
    for (unsigned i = 0; i < material->GetTextureCount(textureType); ++i) {
        aiString str;
        material->GetTexture(textureType, i, &str);

        // TODO: OPTIMIZE, store texture in models as most of mesh reuse them!
        textures.push_back(texture::Texture::create(directory + "/" + str.C_Str()));
    }
    return textures;
}
}