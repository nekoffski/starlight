#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <kc/core/Meta.hpp>

#include "starlight/core/math/Vertex.h"
#include "starlight/core/Core.h"
#include "starlight/core/math/Extent.h"

namespace sl {
namespace detail {

struct GeometryConfigBase {
    std::string name;
    std::string materialName;
    std::vector<u32> indices;
};

template <typename T>
concept ExtentType = kc::core::is_one_of2_v<T, Extent2, Extent3>;

template <typename T>
concept VertexType = kc::core::is_one_of2_v<T, Vertex2, Vertex3>;

template <typename T>
concept VectorType = kc::core::is_one_of2_v<T, Vec2f, Vec3f>;

template <ExtentType Extent, VertexType Vertex, VectorType Vector>
Extent calculateExtent(const std::vector<Vertex>& vertices) {
    Vector min;
    Vector max;

    const auto components = getVectorComponentCount<Vector>();

    for (const auto& vertex : vertices) {
        for (u8 i = 0; i < components; ++i) {
            const auto& component = vertex.position[i];
            if (component < min[i]) min[i] = component;
            if (component > max[i]) max[i] = component;
        }
    }

    return Extent{ min, max };
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

struct GeometryConfig3D final : public detail::GeometryConfigBase {
    std::vector<Vertex3> vertices;

    static GeometryConfig3D generatePlane(const PlaneProperties& props);
    static GeometryConfig3D generateCube(const CubeProperties& props);

    Extent3 calculateExtent() {
        return detail::calculateExtent<Extent3, Vertex3, Vec3f>(vertices);
    }

    void generateTangents();
    void generateNormals();
};

struct GeometryConfig2D final : public detail::GeometryConfigBase {
    std::vector<Vertex2> vertices;

    Extent2 calculateExtent() {
        return detail::calculateExtent<Extent2, Vertex2, Vec2f>(vertices);
    }
};

template <typename T>
concept GeometryConfig =
  kc::core::is_one_of2_v<T, GeometryConfig3D, GeometryConfig2D>;

}  // namespace sl
