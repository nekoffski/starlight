#include "MeshManager.hh"

namespace sl {

MeshManager::MeshManager(
  ResourcePools& resourcePools, MaterialManager& materialManager
) :
    ResourceManager("Me.hh"),
    m_resourcePools(resourcePools), m_materialManager(materialManager) {
    createDefaultGeometries();
}

MeshManager::~MeshManager() {
    forEach([&]([[maybe_unused]] u64, Mesh* mesh) {
        m_resourcePools.destroyMesh(*mesh);
    });
}

Mesh* MeshManager::getDefault3D() { return m_defaultMesh3D; }
Mesh* MeshManager::getDefault2D() { return m_defaultMesh2D; }

void MeshManager::destroyInternals(Mesh* mesh) {}

void MeshManager::createDefaultGeometries() {
    // 3D
    constexpr float scale = 10.0f;

    const auto create3D = [&]() {
        MeshConfig3D config;

        auto& vertices = config.vertices;
        vertices.resize(4);

        vertices[0].position           = { -0.5f * scale, 0.0f, -0.5f * scale };
        vertices[0].textureCoordinates = { 0.0f, 0.0f };

        vertices[1].position           = { 0.5f * scale, 0.0f, 0.5f * scale };
        vertices[1].textureCoordinates = { 1.0f, 1.0f };

        vertices[2].position           = { -0.5f * scale, 0.0f, 0.5f * scale };
        vertices[2].textureCoordinates = { 0.0f, 1.0f };

        vertices[3].position           = { 0.5f * scale, 0.0f, -0.5f * scale };
        vertices[3].textureCoordinates = { 1.0f, 0.0f };

        config.indices = { 2, 1, 0, 1, 3, 0 };
        config.name    = "3d-plane";

        return load(config);
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
        config.name    = "2d-plane";

        return load(config);
    };
    m_defaultMesh2D = create2D();
}

}  // namespace sl
