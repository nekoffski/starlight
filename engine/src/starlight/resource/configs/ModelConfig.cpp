#include "ModelConfig.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "starlight/core/utils/Log.h"

#include "starlight/resource/managers/MaterialManager.h"

namespace sl {

namespace {

class ModelProcessor {
public:
    explicit ModelProcessor(const aiScene* root, const std::string& name);

    std::optional<ModelConfig> process() &&;

private:
    Vertex3 processVertex(
      const aiMesh* model, int index, bool hasNormals, bool hasTangents
    );
    std::vector<u32> processIndices(const aiMesh* model);

    void processNode(const aiNode* node);
    void processModel(const aiMesh* model, int index);
    void processMaterial(aiMaterial* material);

    std::string getMaterialName(const int index);

    const aiScene* m_root;
    std::string m_name;
    ModelConfig m_config;

    int m_verticesWithoutUV;
    Vec3f m_currentMax;
    Vec3f m_currentMin;
};

}  // namespace

std::optional<ModelConfig> ModelConfig::loadOBJ(
  const std::string& name, const std::string& modelsPath, const FileSystem& fs
) {
    const auto fullPath = fmt::format("{}/{}.obj", modelsPath, name);
    LOG_DEBUG("Loading model: {}", fullPath);

    Assimp::Importer importer;

    static const auto flags =
      aiProcess_CalcTangentSpace | aiProcess_Triangulate
      | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
    const auto* scene = importer.ReadFile(fullPath, flags);

    if (not scene) {
        LOG_WARN(
          "Could not load model: {} - {}", fullPath, importer.GetErrorString()
        );
        return {};
    }
    LOG_TRACE("Assimp processed model successfully");
    return ModelProcessor{ scene, name }.process();
}

namespace {

ModelProcessor::ModelProcessor(const aiScene* root, const std::string& name) :
    m_root(root), m_name(name), m_verticesWithoutUV(0) {}

std::optional<ModelConfig> ModelProcessor::process() && {
    LOG_DEBUG(
      "Processing assimp scene; materials={}, models={}, textures={}",
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

Vertex3 ModelProcessor::processVertex(
  const aiMesh* model, int index, bool hasNormals, bool hasTangents
) {
    Vertex3 vertex;
    vertex.position.x = model->mVertices[index].x;
    vertex.position.y = model->mVertices[index].y;
    vertex.position.z = model->mVertices[index].z;

    if (hasNormals) {
        vertex.normal.x = model->mNormals[index].x;
        vertex.normal.y = model->mNormals[index].y;
        vertex.normal.z = model->mNormals[index].z;
    }

    if (hasTangents) {
        vertex.tangent.x = model->mTangents[index].x;
        vertex.tangent.y = model->mTangents[index].y;
        vertex.tangent.z = model->mTangents[index].z;
        vertex.tangent.w = 1.0f;
    }

    if (auto& color = model->mColors[0]; color) {
        vertex.color.x = color[index].r;
        vertex.color.y = color[index].g;
        vertex.color.z = color[index].b;
        vertex.color.w = color[index].a;
    }

    if (auto& coordinates = model->mTextureCoords[0]; coordinates) {
        vertex.textureCoordinates.x = coordinates[index].x;
        vertex.textureCoordinates.y = coordinates[index].y;
    } else {
        ++m_verticesWithoutUV;
    }
    return vertex;
}

std::vector<u32> ModelProcessor::processIndices(const aiMesh* model) {
    std::vector<u32> indices;
    for (int i = 0; i < model->mNumFaces; ++i) {
        auto& face = model->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }
    return indices;
}

void ModelProcessor::processNode(const aiNode* node) {
    LOG_DEBUG(
      "Processing assimp node, meshes={}, children={}", node->mNumMeshes,
      node->mNumChildren
    );
    m_config.meshes.reserve(node->mNumMeshes);
    for (int i = 0; i < node->mNumMeshes; ++i)
        processModel(m_root->mMeshes[node->mMeshes[i]], i);
    for (int i = 0; i < node->mNumChildren; ++i) processNode(node->mChildren[i]);
}

static inline std::string toString(const aiString& str) {
    return std::string(str.C_Str());
}

void ModelProcessor::processModel(const aiMesh* model, int index) {
    const bool hasNormals   = model->HasNormals();
    const bool hasTangents  = model->HasTangentsAndBitangents();
    const auto materialName = getMaterialName(model->mMaterialIndex);

    LOG_DEBUG(
      "Processing model; vertices={}, faces={}, has-normals={}, has-tangents={}, material='{}'",
      model->mNumVertices, model->mNumFaces, hasNormals, hasTangents, materialName
    );

    MeshConfig3D config;
    config.vertices.reserve(model->mNumVertices);
    config.name = fmt::format("{}_mesh_{}", m_name, index);

    for (int i = 0; i < model->mNumVertices; ++i)
        config.vertices.push_back(processVertex(model, i, hasNormals, hasTangents));
    config.indices = processIndices(model);

    if (not hasNormals) config.generateNormals();
    if (not hasTangents) config.generateTangents();

    m_config.meshes[materialName].push_back(std::move(config));
}

void ModelProcessor::processMaterial(aiMaterial* material) {
    const auto name = toString(material->GetName());
    LOG_DEBUG(
      "Processing material: {}, properties={}", name, material->mNumProperties
    );
    for (int i = 0; i < material->mNumProperties; ++i)
        LOG_TRACE("\t{}", toString(material->mProperties[i]->mKey));

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

    float shininess = 0.0f;
    if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
        config.shininess = shininess;
        LOG_DEBUG("\tShininess={}", config.shininess);
    } else {
        LOG_WARN("\tCould not load shininess, setting default={}", config.shininess);
    }

    aiString str;

    if (auto textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
        textureCount > 0) {
        LOG_TRACE("\t{} diffuse textures", textureCount);
        if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), str) == AI_SUCCESS) {
            config.diffuseMap = toString(str);
            LOG_DEBUG("\tDiffuse map='{}'", config.diffuseMap);
        } else {
            LOG_WARN(
              "\tCould not load diffuse map, setting default='{}'", config.diffuseMap
            );
        }
    } else {
        LOG_TRACE("\tNo diffuse textures, setting default='{}'", config.diffuseMap);
    }

    if (auto textureCount = material->GetTextureCount(aiTextureType_SPECULAR);
        textureCount > 0) {
        LOG_TRACE("\t{} specular textures", textureCount);
        if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), str) == AI_SUCCESS) {
            config.specularMap = toString(str);
            LOG_DEBUG("\tSpecular map='{}'", config.specularMap);
        } else {
            LOG_WARN(
              "\tCould not load specular map, setting default='{}'",
              config.specularMap
            );
        }
    } else {
        LOG_TRACE(
          "\tNo specular textures, setting default='{}'", config.specularMap
        );
    }

    if (auto textureCount = material->GetTextureCount(aiTextureType_HEIGHT);
        textureCount > 0) {
        LOG_TRACE("\t{} height maps", textureCount);
        if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT, 0), str) == AI_SUCCESS) {
            config.normalMap = toString(str);
            LOG_DEBUG("\tNormal map='{}'", config.normalMap);
        } else {
            LOG_WARN(
              "\tCould not load normal map, setting default='{}'", config.normalMap
            );
        }
    } else {
        LOG_TRACE("\tNo height maps, setting default='{}'", config.normalMap);
    }
    m_config.materials.push_back(config);
}

std::string ModelProcessor::getMaterialName(const int index) {
    return toString(m_root->mMaterials[index]->GetName());
}

}  // namespace
}  // namespace sl
