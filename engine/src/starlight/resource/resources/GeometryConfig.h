#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <kc/core/Meta.hpp>

#include "starlight/core/math/Vertex.h"
#include "starlight/core/Core.h"

namespace sl {
namespace detail {

struct GeometryConfigBase {
    std::string name;
    std::string materialName;
    std::vector<u32> indices;

    Vec3f center;
    Vec3f minExtents;
    Vec3f maxExtents;
};
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

    void generateTangents();
    void generateNormals();
};

struct GeometryConfig2D final : public detail::GeometryConfigBase {
    std::vector<Vertex2> vertices;
};

template <typename T>
concept GeometryConfig =
  kc::core::is_one_of2_v<T, GeometryConfig3D, GeometryConfig2D>;

}  // namespace sl