#include "ProceduralGeometry.hh"

#include <array>

#include "GeometryDataBuilder.hh"
#include "Vertex.hh"

namespace sl {

GeometryData ProceduralGeometry::cube() {
    struct Face {
        Vec3f normal;
        Vec3f right;
        Vec3f up;
    };
    const std::array faces{
        Face{{1, 0, 0}, {0, 0, -1}, {0, 1, 0}},
        Face{{-1, 0, 0}, {0, 0, 1}, {0, 1, 0}},
        Face{{0, 1, 0}, {1, 0, 0}, {0, 0, -1}},
        Face{{0, -1, 0}, {1, 0, 0}, {0, 0, 1}},
        Face{{0, 0, 1}, {1, 0, 0}, {0, 1, 0}},
        Face{{0, 0, -1}, {-1, 0, 0}, {0, 1, 0}},
    };
    const std::array textureCoordinates{
        Vec2f{0, 0}, Vec2f{1, 0}, Vec2f{1, 1}, Vec2f{0, 1}
    };
    GeometryDataBuilder<Vertex3> builder;
    u32 firstVertex = 0;
    for (const auto& face : faces) {
        for (const auto uv : textureCoordinates) {
            const auto position = face.normal * 0.5f +
                                  face.right * (uv.x - 0.5f) +
                                  face.up * (uv.y - 0.5f);
            builder.addVertex(position, face.normal, uv);
        }
        builder.addIndices(
            {firstVertex, firstVertex + 1, firstVertex + 2, firstVertex,
             firstVertex + 2, firstVertex + 3}
        );
        firstVertex += textureCoordinates.size();
    }
    return builder.build();
}

}  // namespace sl
