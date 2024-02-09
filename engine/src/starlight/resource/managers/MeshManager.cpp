#include "MeshManager.h"

namespace sl {

MeshManager::MeshManager(
  ResourcePools& resourcePools, MaterialManager& materialManager
) :
    m_resourcePools(resourcePools),
    m_materialManager(materialManager) {
    createDefaultGeometries();
}

MeshManager::~MeshManager() { destroyAll(); }

Mesh* MeshManager::acquire(u32 id) {
    if (auto mesh = m_meshes.find(id); mesh != m_meshes.end()) return mesh->second;
    LOG_WARN("Mesh with id='{}' not found", id);
    return nullptr;
}

void MeshManager::destroy(uint32_t id) {
    if (auto mesh = m_meshes.find(id); mesh == m_meshes.end()) {
        LOG_WARN("Trying to destroy mesh '{}' that couldn't be found", mesh->first);
        return;
    } else {
        m_resourcePools.destroyMesh(*mesh->second);
    }
}

void MeshManager::destroyAll() {
    LOG_TRACE("Destroying all meshes");
    for (auto& mesh : m_meshes | std::views::values)
        m_resourcePools.destroyMesh(*mesh);
    m_meshes.clear();
}

Mesh* MeshManager::getDefault3D() { return m_defaultMesh3D; }
Mesh* MeshManager::getDefault2D() { return m_defaultMesh2D; }

void MeshManager::createDefaultGeometries() {
    // 3D
    constexpr float scale = 10.0f;

    const auto create3D = [&]() {
        MeshConfig3D config;

        auto& vertices = config.vertices;
        vertices.resize(4);

        vertices[0].position           = { -0.5f * scale, -0.5f * scale, 0.0f };
        vertices[0].textureCoordinates = { 0.0f, 0.0f };

        vertices[1].position           = { 0.5f * scale, 0.5f * scale, 0.0f };
        vertices[1].textureCoordinates = { 1.0f, 1.0f };

        vertices[2].position           = { -0.5f * scale, 0.5f * scale, 0.0f };
        vertices[2].textureCoordinates = { 0.0f, 1.0f };

        vertices[3].position           = { 0.5f * scale, -0.5f * scale, 0.0f };
        vertices[3].textureCoordinates = { 1.0f, 0.0f };

        config.indices = { 0, 1, 2, 0, 3, 1 };

        Mesh::Properties props{
            "default-3d-mesh", m_materialManager.getDefaultMaterial()
        };
        return m_resourcePools.createMesh(
          props, vertices, config.indices, config.calculateExtent()
        );
    };
    m_defaultMesh3D = create3D();

    // 2D
    const auto create2D = [&]() {
        MeshConfig2D config;

        auto& vertices = config.vertices;
        vertices.resize(4);

        vertices[0].position           = { -0.5f * scale, -0.5f * scale };
        vertices[0].textureCoordinates = { 0.0f, 0.0f };

        vertices[1].position           = { 0.5f * scale, 0.5f * scale };
        vertices[1].textureCoordinates = { 1.0f, 1.0f };

        vertices[2].position           = { -0.5f * scale, 0.5f * scale };
        vertices[2].textureCoordinates = { 0.0f, 1.0f };

        vertices[3].position           = { 0.5f * scale, -0.5f * scale };
        vertices[3].textureCoordinates = { 1.0f, 0.0f };

        config.indices = { 2, 1, 0, 3, 0, 1 };

        Mesh::Properties props{
            "default-2d-mesh", m_materialManager.getDefaultMaterial()
        };
        return m_resourcePools.createMesh(
          props, vertices, config.indices, config.calculateExtent()
        );
    };
    m_defaultMesh2D = create2D();
}

}  // namespace sl
