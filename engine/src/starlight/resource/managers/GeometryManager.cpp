#include "GeometryManager.h"

namespace sl {

GeometryManager::GeometryManager(
  RendererProxy& rendererProxy, MaterialManager& materialManager
) :
    m_rendererProxy(rendererProxy),
    m_materialManager(materialManager) {
    for (auto& geometry : m_geometries) invalidateEntry(geometry);
    createDefaultGeometries();
}

Geometry* GeometryManager::acquire(uint32_t id) {
    if (id != invalidId && m_geometries[id].id != invalidId)
        return &m_geometries[id];

    LOG_WARN("Geometry with id='{}' not found", id);
    return nullptr;
}

Geometry* GeometryManager::findSlot() {
    for (uint32_t i = 0; i < m_geometries.size(); ++i) {
        if (auto slot = &m_geometries[i]; slot->id == invalidId) {
            slot->id = i;
            return slot;
        }
    }
    return nullptr;
}

void GeometryManager::destroy(uint32_t id) {
    if (auto& geometry = m_geometries[id]; geometry.id != invalidId) {
        LOG_WARN("Trying to destroy geometry '{}' that is invalid", id);
        return;
    } else {
        destroy(geometry);
    }
}

void GeometryManager::destroy(Geometry& geometry) {
    m_rendererProxy.releaseGeometryResources(geometry);
    invalidateEntry(geometry);
}

Geometry* GeometryManager::getDefault3D() { return &m_defaultGeometry3D; }
Geometry* GeometryManager::getDefault2D() { return &m_defaultGeometry2D; }

void GeometryManager::invalidateEntry(Geometry& geometry) {
    geometry.generation = 0;
    geometry.id         = invalidId;
    geometry.internalId = invalidId;
    geometry.material   = nullptr;
    geometry.name.clear();
}

void GeometryManager::createDefaultGeometries() {
    m_defaultGeometry2D.name = "default-2d-geometry";
    m_defaultGeometry3D.name = "default-3d-geometry";

    // 3D
    constexpr float scale = 10.0f;

    const auto create3D = [&]() {
        std::array<Vertex3, 4> vertices;

        vertices[0].position           = { -0.5f * scale, -0.5f * scale, 0.0f };
        vertices[0].textureCoordinates = { 0.0f, 0.0f };

        vertices[1].position           = { 0.5f * scale, 0.5f * scale, 0.0f };
        vertices[1].textureCoordinates = { 1.0f, 1.0f };

        vertices[2].position           = { -0.5f * scale, 0.5f * scale, 0.0f };
        vertices[2].textureCoordinates = { 0.0f, 1.0f };

        vertices[3].position           = { 0.5f * scale, -0.5f * scale, 0.0f };
        vertices[3].textureCoordinates = { 1.0f, 0.0f };

        std::array<uint32_t, 6> indices = { 0, 1, 2, 0, 3, 1 };

        m_rendererProxy.acquireGeometryResources(
          m_defaultGeometry3D, vertices, indices
        );
        m_defaultGeometry3D.material = m_materialManager.getDefaultMaterial();
    };
    create3D();

    // 2D
    const auto create2D = [&]() {
        std::array<Vertex2, 4> vertices;

        vertices[0].position           = { -0.5f * scale, -0.5f * scale };
        vertices[0].textureCoordinates = { 0.0f, 0.0f };

        vertices[1].position           = { 0.5f * scale, 0.5f * scale };
        vertices[1].textureCoordinates = { 1.0f, 1.0f };

        vertices[2].position           = { -0.5f * scale, 0.5f * scale };
        vertices[2].textureCoordinates = { 0.0f, 1.0f };

        vertices[3].position           = { 0.5f * scale, -0.5f * scale };
        vertices[3].textureCoordinates = { 1.0f, 0.0f };

        std::array<uint32_t, 6> indices = { 2, 1, 0, 3, 0, 1 };

        m_rendererProxy.acquireGeometryResources(
          m_defaultGeometry2D, vertices, indices
        );
        m_defaultGeometry2D.material = m_materialManager.getDefaultMaterial();
    };
    create2D();
}

}  // namespace sl
