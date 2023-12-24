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

    // TODO: unify
    Extent3 calculateExtent() {
        Vec3f min{ 0.0f };
        Vec3f max{ 0.0f };

        for (const auto& vertex : vertices) {
            for (u8 i = 0; i < 3; ++i) {
                const auto& component = vertex.position[i];
                if (component < min[i]) min[i] = component;
                if (component > max[i]) max[i] = component;
            }
        }
        return Extent3{ min, max };
    }

    void generateTangents();
    void generateNormals();
};

struct GeometryConfig2D final : public detail::GeometryConfigBase {
    std::vector<Vertex2> vertices;

    Extent2 calculateExtent() {
        Vec2f min{ 0.0f };
        Vec2f max{ 0.0f };

        for (const auto& vertex : vertices) {
            for (u8 i = 0; i < 2; ++i) {
                const auto& component = vertex.position[i];
                if (component < min[i]) min[i] = component;
                if (component > max[i]) max[i] = component;
            }
        }
        return Extent2{ min, max };
    }
};

template <typename T>
concept GeometryConfig =
  kc::core::is_one_of2_v<T, GeometryConfig3D, GeometryConfig2D>;

}  // namespace sl