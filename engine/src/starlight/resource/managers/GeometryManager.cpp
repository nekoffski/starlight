#include "GeometryManager.h"

namespace sl {

GeometryManager::GeometryManager(
  RendererProxy& rendererProxy, MaterialManager& materialManager
) :
    m_rendererProxy(rendererProxy),
    m_materialManager(materialManager) {
    createDefaultGeometries();
}

Geometry* GeometryManager::acquire(u32 id) {
    if (auto geometry = m_geometries.find(id); geometry != m_geometries.end())
        return geometry->second;
    LOG_WARN("Geometry with id='{}' not found", id);
    return nullptr;
}

void GeometryManager::destroy(uint32_t id) {
    if (auto geometry = m_geometries.find(id); geometry == m_geometries.end()) {
        LOG_WARN(
          "Trying to destroy geometry '{}' that couldn't be found", geometry->first
        );
        return;
    } else {
        m_rendererProxy.destroyGeometry(*geometry->second);
    }
}

Geometry* GeometryManager::getDefault3D() { return m_defaultGeometry3D; }
Geometry* GeometryManager::getDefault2D() { return m_defaultGeometry2D; }

void GeometryManager::createDefaultGeometries() {
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

        Geometry::Properties props{
            "default-3d-geometry", m_materialManager.getDefaultMaterial()
        };
        return m_rendererProxy.createGeometry(props, vertices, indices);
    };
    m_defaultGeometry3D = create3D();

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

        Geometry::Properties props{
            "default-2d-geometry", m_materialManager.getDefaultMaterial()
        };
        return m_rendererProxy.createGeometry(props, vertices, indices);
    };
    m_defaultGeometry2D = create2D();
}

}  // namespace sl
