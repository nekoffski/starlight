#include "MeshConfig.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "starlight/core/utils/Log.h"

#include "starlight/resource/managers/MaterialManager.h"

namespace sl {

namespace {

class MeshProcessor {
public:
    explicit MeshProcessor(const aiScene* root, const std::string& name);

    std::optional<MeshConfig> process() &&;

private:
    Vertex3 processVertex(
      const aiMesh* mesh, int index, bool hasNormals, bool hasTangents
    );
    std::vector<u32> processIndices(const aiMesh* mesh);

    void processNode(const aiNode* node);
    void processMesh(const aiMesh* mesh, int index);
    void processMaterial(aiMaterial* material);

    std::string getMaterialName(const int index);

    const aiScene* m_root;
    std::string m_name;
    MeshConfig m_config;

    int m_verticesWithoutUV;
    Vec3f m_currentMax;
    Vec3f m_currentMin;
};

}  // namespace

std::optional<MeshConfig> MeshConfig::loadOBJ(
  const std::string& name, const std::string& modelsPath, const FileSystem& fs
) {
    const auto fullPath = fmt::format("{}/{}.obj", modelsPath, name);
    LOG_DEBUG("Loading mesh: {}", fullPath);

    Assimp::Importer importer;

    static const auto flags =
      aiProcess_CalcTangentSpace | aiProcess_Triangulate
      | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
    const auto* scene = importer.ReadFile(fullPath, flags);

    if (not scene) {
        LOG_WARN(
          "Could not load mesh: {} - {}", fullPath, importer.GetErrorString()
        );
        return {};
    }
    LOG_TRACE("Assimp processed mesh successfully");
    return MeshProcessor{ scene, name }.process();
}

namespace {

MeshProcessor::MeshProcessor(const aiScene* root, const std::string& name) :
    m_root(root), m_name(name), m_verticesWithoutUV(0) {}

std::optional<MeshConfig> MeshProcessor::process() && {
    LOG_DEBUG(
      "Processing assimp scene; materials={}, meshes={}, textures={}",
      m_root->mNumMaterials, m_root->mNumMeshes, m_root->mNumTextures
    );
    m_config.materials.reserve(m_root->mNumMaterials);
    for (int i = 0; i < m_root->mNumMaterials; ++i)
        processMaterial(m_root->mMaterials[i]);
    processNode(m_root->mRootNode);

    if (m_verticesWithoutUV > 0) {
        LOG_WARN(
          "{} vertices don't have UV coordinates, this may result as a visual artifact",
          m_verticesWithoutUV
        );
    }
    return m_config;
}

Vertex3 MeshProcessor::processVertex(
  const aiMesh* mesh, int index, bool hasNormals, bool hasTangents
) {
    Vertex3 vertex;
    vertex.position.x = mesh->mVertices[index].x;
    vertex.position.y = mesh->mVertices[index].y;
    vertex.position.z = mesh->mVertices[index].z;

    if (hasNormals) {
        vertex.normal.x = mesh->mNormals[index].x;
        vertex.normal.y = mesh->mNormals[index].y;
        vertex.normal.z = mesh->mNormals[index].z;
    }

    if (hasTangents) {
        vertex.tangent.x = mesh->mTangents[index].x;
        vertex.tangent.y = mesh->mTangents[index].y;
        vertex.tangent.z = mesh->mTangents[index].z;
        vertex.tangent.w = 1.0f;
    }

    if (auto& color = mesh->mColors[0]; color) {
        vertex.color.x = color[index].r;
        vertex.color.y = color[index].g;
        vertex.color.z = color[index].b;
        vertex.color.w = color[index].a;
    }

    if (auto& coordinates = mesh->mTextureCoords[0]; coordinates) {
        vertex.textureCoordinates.x = coordinates[index].x;
        vertex.textureCoordinates.y = coordinates[index].y;
    } else {
        ++m_verticesWithoutUV;
    }
    return vertex;
}

std::vector<u32> MeshProcessor::processIndices(const aiMesh* mesh) {
    std::vector<u32> indices;
    for (int i = 0; i < mesh->mNumFaces; ++i) {
        auto& face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }
    return indices;
}

void MeshProcessor::processNode(const aiNode* node) {
    LOG_DEBUG(
      "Processing assimp nod;, meshes={}, children={}", node->mNumMeshes,
      node->mNumChildren
    );
    m_config.geometries.reserve(node->mNumMeshes);
    for (int i = 0; i < node->mNumMeshes; ++i)
        processMesh(m_root->mMeshes[node->mMeshes[i]], i);
    for (int i = 0; i < node->mNumChildren; ++i) processNode(node->mChildren[i]);
}

static inline std::string toString(const aiString& str) {
    return std::string(str.C_Str());
}

void MeshProcessor::processMesh(const aiMesh* mesh, int index) {
    const bool hasNormals   = mesh->HasNormals();
    const bool hasTangents  = mesh->HasTangentsAndBitangents();
    const auto materialName = getMaterialName(mesh->mMaterialIndex);

    LOG_DEBUG(
      "Processing mesh; vertices={}, faces={}, has-normals={}, has-tangents={}, material='{}'",
      mesh->mNumVertices, mesh->mNumFaces, hasNormals, hasTangents, materialName
    );

    GeometryConfig3D config;
    config.vertices.reserve(mesh->mNumVertices);
    config.materialName = materialName;
    config.name         = fmt::format("{}_mesh_{}", m_name, index);

    for (int i = 0; i < mesh->mNumVertices; ++i)
        config.vertices.push_back(processVertex(mesh, i, hasNormals, hasTangents));
    config.indices = processIndices(mesh);

    if (not hasNormals) config.generateNormals();
    if (not hasTangents) config.generateTangents();

    m_config.geometries.push_back(std::move(config));
}

void MeshProcessor::processMaterial(aiMaterial* material) {
    const auto name = toString(material->GetName());
    LOG_DEBUG(
      "Processing material: {}, properties={}", name, material->mNumProperties
    );

    auto config = MaterialConfig::createDefault(name);

    aiColor3D color;
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
        config.diffuseColor.x = color.r;
        config.diffuseColor.y = color.g;
        config.diffuseColor.z = color.b;
        config.diffuseColor.a = 1.0f;
        LOG_DEBUG("\tDiffuse color={}", config.diffuseColor);
    } else {
        LOG_WARN(
          "\tCould not load diffuse color, setting default={}", config.diffuseColor
        );
    }

    float shininess;
    if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
        config.shininess = shininess;
        LOG_DEBUG("\tShininess={}", config.shininess);
    } else {
        LOG_WARN("\tCould not load shininess, setting default={}", config.shininess);
    }

    aiString str;
    if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), str) == AI_SUCCESS) {
        config.diffuseMap = toString(str);
        LOG_DEBUG("\tDiffuse map='{}'", config.diffuseMap);
    } else {
        LOG_WARN(
          "\tCould not load diffuse map, setting default='{}'", config.diffuseMap
        );
    }

    if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), str) == AI_SUCCESS) {
        config.specularMap = toString(str);
        LOG_DEBUG("\tSpecular map='{}'", config.specularMap);
    } else {
        LOG_WARN(
          "\tCould not load specular map, setting default='{}'", config.specularMap
        );
    }

    if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT, 0), str) == AI_SUCCESS) {
        config.normalMap = toString(str);
        LOG_DEBUG("\tNormal map='{}'", config.normalMap);
    } else {
        LOG_WARN(
          "\tCould not load normal map, setting default='{}'", config.normalMap
        );
    }

    m_config.materials.push_back(config);
}

std::string MeshProcessor::getMaterialName(const int index) {
    return toString(m_root->mMaterials[index]->GetName());
}

}  // namespace
}  // namespace sl
