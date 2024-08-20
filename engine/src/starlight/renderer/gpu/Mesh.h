#pragma once

#include <string>
#include <span>

#include <kc/core/Meta.hpp>

#include "starlight/core/math/Extent.hpp"
#include "starlight/core/math/Core.h"
#include "starlight/renderer/Material.h"
#include "starlight/renderer/Resource.hpp"

namespace sl {
namespace detail {

template <typename T>
concept ExtentType = kc::core::is_one_of2_v<T, Extent2, Extent3>;

template <typename T>
concept VertexType = kc::core::is_one_of2_v<T, Vertex2, Vertex3>;

template <typename T>
concept VectorType = kc::core::is_one_of2_v<T, Vec2<f32>, Vec3<f32>>;

struct MeshConfigBase {
    std::string name;
    std::vector<u32> indices;
};

template <ExtentType Extent, VertexType Vertex, VectorType Vector>
Extent calculateExtent(const std::vector<Vertex>& vertices) {
    Vector min(0.0f);
    Vector max(0.0f);

    const auto components = getVectorComponentCount<Vector>();

    for (const auto& vertex : vertices) {
        for (u8 i = 0; i < components; ++i) {
            const auto& component = vertex.position[i];
            if (component < min[i]) min[i] = component;
            if (component > max[i]) max[i] = component;
        }
    }

    return Extent(min, max);
}

}  // namespace detail

struct PlaneProperties {
    float width;
    float height;
    u32 xSegments;
    u32 ySegments;
    u32 xTile;
    u32 yTile;
    std::string_view name;
    std::string_view materialName;
};

struct CubeProperties {
    float width;
    float height;
    float depth;
    u32 xTile;
    u32 yTile;
    std::string_view name;
    std::string_view materialName;
};

struct MeshConfig3D final : public detail::MeshConfigBase {
    std::vector<Vertex3> vertices;

    static MeshConfig3D generatePlane(const PlaneProperties& props);
    static MeshConfig3D generateCube(const CubeProperties& props);

    Extent3 calculateExtent() const {
        return detail::calculateExtent<Extent3, Vertex3, Vec3<f32>>(vertices);
    }

    void generateTangents();
    void generateNormals();
};

struct MeshConfig2D final : public detail::MeshConfigBase {
    std::vector<Vertex2> vertices;

    Extent2 calculateExtent() const {
        return detail::calculateExtent<Extent2, Vertex2, Vec2<f32>>(vertices);
    }
};

template <typename T>
concept MeshConfig = kc::core::is_one_of2_v<T, MeshConfig3D, MeshConfig2D>;

class Mesh {
public:
    struct Properties {
        std::string name;
    };

    struct Data {
        u64 vertexSize;
        u64 vertexCount;
        const void* vertexData;
        std::span<const u32> indices;
        Extent3 extent;
    };

    struct BufferDescription {
        u64 vertexCount;
        u64 vertexElementSize;
        u64 vertexBufferOffset;
        u64 indexCount;
        u64 indexElementSize;
        u64 indexBufferOffset;

        u64 indicesTotalSize() const { return indexCount * indexElementSize; }
        u64 verticesTotalSize() const { return vertexCount * vertexElementSize; }
    };

    virtual ~Mesh() = default;

    static ResourceRef<Mesh> load(const MeshConfig2D& config);
    static ResourceRef<Mesh> load(const MeshConfig3D& config);
    static ResourceRef<Mesh> find(const std::string& name);

    static ResourceRef<Mesh> getCube();

    const BufferDescription& getDataDescription() const;
    const Properties& getProperties() const;
    const Extent3& getExtent() const;

    u32 getId() const;

protected:
    explicit Mesh(const Properties& props, const Data& data, u32 id);

    u32 m_id;

    Properties m_props;
    BufferDescription m_dataDescription;
    Extent3 m_extent;
};

class MeshManager
    : public ResourceManager<Mesh>,
      public kc::core::Singleton<MeshManager> {
public:
    explicit MeshManager(RendererBackend& renderer);

    ResourceRef<Mesh> load(const MeshConfig2D& config);
    ResourceRef<Mesh> load(const MeshConfig3D& config);

private:
    RendererBackend& m_renderer;
};

}  // namespace sl
