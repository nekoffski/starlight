#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>
#include <kc/core/Meta.hpp>

#include "starlight/core/utils/FileSystem.h"
#include "starlight/core/memory/Memory.hpp"

#include "starlight/renderer/Geometry.h"
#include "starlight/renderer/Material.h"
#include "starlight/renderer/gpu/RendererProxy.h"
#include "starlight/core/math/Vertex3.h"

#include "MaterialManager.h"

constexpr int maxGeometries = 4096;

// TODO: refactor

namespace sl {

namespace detail {

struct GeometryPropertiesBase {
    std::string name;
    std::string materialName;
    std::vector<uint32_t> indices;
};
}  // namespace detail

struct GeometryProperties3D final : public detail::GeometryPropertiesBase {
    std::vector<Vertex3> vertices;
};

struct GeometryProperties2D final : public detail::GeometryPropertiesBase {
    std::vector<Vertex2> vertices;
};

template <typename T>
concept GeometryProperties =
  kc::core::is_one_of2_v<T, GeometryProperties3D, GeometryProperties2D>;

struct PlaneProperties {
    float width;
    float height;
    uint32_t xSegments;
    uint32_t ySegments;
    uint32_t xTile;
    uint32_t yTile;
    std::string_view name;
    std::string_view materialName;
};

// TODO: pass objects as views instead of references
class GeometryManager {
public:
    explicit GeometryManager(
      RendererProxy& resourceProxy, MaterialManager& materialManager
    ) :
        m_rendererProxy(resourceProxy),
        m_materialManager(materialManager) {
        for (auto& geometry : m_geometries) invalidateEntry(geometry);
        createDefaultGeometries();
    }

    Geometry* acquire(uint32_t id) {
        if (id != invalidId && m_geometries[id].id != invalidId)
            return &m_geometries[id];

        LOG_WARN("Geometry with id='{}' not found", id);
        return nullptr;
    }

    Geometry* load(GeometryProperties auto& props) {
        const auto findSlot = [&]() -> Geometry* {
            for (uint32_t i = 0; i < m_geometries.size(); ++i) {
                if (auto slot = &m_geometries[i]; slot->id == invalidId) {
                    slot->id = i;
                    return slot;
                }
            }
            return nullptr;
        };

        auto geometry = findSlot();

        if (not geometry) {
            LOG_ERROR(
              "Could not find empty slot for geometry, consider changing configuration"
            );
            return nullptr;
        }

        if (not createGeometry(props, *geometry)) {
            LOG_ERROR("Could not create geometry");
            return nullptr;
        }

        return geometry;
    }

    void destroy(uint32_t id) {
        if (auto& geometry = m_geometries[id]; geometry.id != invalidId) {
            LOG_WARN("Trying to destroy geometry '{}' that is invalid", id);
            return;
        } else {
            destroy(geometry);
        }
    }

    void destroy(Geometry& geometry) {
        m_rendererProxy.releaseGeometryResources(geometry);
        invalidateEntry(geometry);
    }

    Geometry* getDefault3D() { return &m_defaultGeometry3D; }
    Geometry* getDefault2D() { return &m_defaultGeometry2D; }

    static GeometryProperties3D generatePlaneGeometryProperties(
      PlaneProperties& props
    ) {
        // check for properties
        GeometryProperties3D out;

        const auto vertexCount = props.xSegments * props.ySegments * 4;
        const auto indexCount  = props.xSegments * props.ySegments * 6;

        out.vertices.resize(vertexCount);
        out.indices.resize(indexCount);

        float segWidth   = (float)props.width / props.xSegments;
        float segHeight  = (float)props.height / props.ySegments;
        float halfWidth  = props.width / 2.0f;
        float halfHeight = props.height / 2.0f;

        for (uint32_t y = 0; y < props.ySegments; ++y) {
            for (uint32_t x = 0; x < props.xSegments; ++x) {
                float minX = (x * segWidth) - halfWidth;
                float minY = (y * segHeight) - halfHeight;
                float maxX = minX + segWidth;
                float maxY = minY + segHeight;

                float minUVX = (x / (float)props.xSegments) * props.xTile;
                float minUVY = (y / (float)props.ySegments) * props.yTile;
                float maxUVX = ((x + 1.0f) / (float)props.xSegments) * props.xTile;
                float maxUVY = ((y + 1.0f) / (float)props.ySegments) * props.yTile;

                uint32_t vOffset = ((y * props.xSegments) + x) * 4;

                auto v0 = &out.vertices[vOffset + 0];
                auto v1 = &out.vertices[vOffset + 1];
                auto v2 = &out.vertices[vOffset + 2];
                auto v3 = &out.vertices[vOffset + 3];

                v0->position           = glm::vec3{ minX, minY, 0.0f };
                v0->textureCoordinates = glm::vec2{ minUVX, minUVY };

                v1->position           = glm::vec3{ maxX, maxY, 0.0f };
                v1->textureCoordinates = glm::vec2{ minUVX, maxUVY };

                v2->position           = glm::vec3{ minX, maxY, 0.0f };
                v2->textureCoordinates = glm::vec2{ minUVX, maxUVY };

                v3->position           = glm::vec3{ maxX, minY, 0.0f };
                v3->textureCoordinates = glm::vec2{ maxUVX, minUVY };

                uint32_t iOffset = ((y * props.xSegments) + x) * 6;

                out.indices[iOffset + 0] = vOffset + 0;
                out.indices[iOffset + 1] = vOffset + 1;
                out.indices[iOffset + 2] = vOffset + 2;
                out.indices[iOffset + 3] = vOffset + 0;
                out.indices[iOffset + 4] = vOffset + 3;
                out.indices[iOffset + 5] = vOffset + 1;
            }
        }
        out.name         = props.name;
        out.materialName = props.materialName;

        return out;
    }

    //    private:
    void invalidateEntry(Geometry& geometry) {
        geometry.generation = 0;
        geometry.id         = invalidId;
        geometry.internalId = invalidId;
        geometry.material   = nullptr;
        geometry.name.clear();
    }

    void createDefaultGeometries() {
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

    bool createGeometry(GeometryProperties auto& props, Geometry& geometry) {
        geometry.name = props.name;

        m_rendererProxy.acquireGeometryResources(
          geometry, props.vertices, props.indices
        );  // TODO: check if succeed

        geometry.material =
          m_materialManager.acquire(props.materialName)
            ?: m_materialManager.getDefaultMaterial();

        return true;
    }

    RendererProxy& m_rendererProxy;
    MaterialManager& m_materialManager;
    std::array<Geometry, maxGeometries> m_geometries;

    Geometry m_defaultGeometry3D;
    Geometry m_defaultGeometry2D;
};

}  // namespace sl
