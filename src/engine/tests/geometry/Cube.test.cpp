#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <cstring>
#include <set>

#include "starlight/geometry/ProceduralGeometry.hh"
#include "starlight/geometry/Vertex.hh"

namespace {

TEST(Cube, HasCompleteOutwardFacesWithIndependentNormalsAndUvs) {
    const auto data = sl::ProceduralGeometry::cube();
    ASSERT_EQ(data.vertexCount, 24);
    ASSERT_EQ(data.indexCount, 36);
    EXPECT_EQ(data.vertexLayout, sl::Vertex3::layout());
    std::array<sl::Vertex3, 24> vertices;
    std::array<sl::u32, 36> indices;
    ASSERT_EQ(data.vertexData.size(), sizeof(vertices));
    ASSERT_EQ(data.indexData.size(), sizeof(indices));
    std::memcpy(vertices.data(), data.vertexData.data(), sizeof(vertices));
    std::memcpy(indices.data(), data.indexData.data(), sizeof(indices));
    std::set<std::array<int, 3>> faceNormals;
    for (size_t face = 0; face < 6; ++face) {
        const auto normal = vertices[face * 4].normal;
        EXPECT_FLOAT_EQ(glm::length(normal), 1);
        faceNormals.insert({int(normal.x), int(normal.y), int(normal.z)});
        std::set<std::array<int, 2>> faceUvs;
        for (size_t corner = 0; corner < 4; ++corner) {
            const auto& vertex = vertices[face * 4 + corner];
            EXPECT_EQ(vertex.normal, normal);
            for (size_t axis = 0; axis < 3; ++axis) {
                EXPECT_FLOAT_EQ(std::abs(vertex.position[axis]), 0.5f);
                EXPECT_TRUE(normal[axis] == 0 || std::abs(normal[axis]) == 1);
            }
            EXPECT_FLOAT_EQ(glm::dot(vertex.position, normal), 0.5f);
            EXPECT_TRUE(vertex.texCoord.x == 0 || vertex.texCoord.x == 1);
            EXPECT_TRUE(vertex.texCoord.y == 0 || vertex.texCoord.y == 1);
            faceUvs.insert({int(vertex.texCoord.x), int(vertex.texCoord.y)});
        }
        EXPECT_EQ(faceUvs.size(), 4);
        for (size_t triangle = 0; triangle < 2; ++triangle) {
            const auto start = face * 6 + triangle * 3;
            for (size_t index = start; index < start + 3; ++index) {
                ASSERT_LT(indices[index], vertices.size());
                EXPECT_EQ(indices[index] / 4, face);
            }
            const auto a = vertices[indices[start]].position;
            const auto b = vertices[indices[start + 1]].position;
            const auto c = vertices[indices[start + 2]].position;
            EXPECT_FLOAT_EQ(glm::dot(glm::cross(b - a, c - a), normal), 1);
        }
    }
    EXPECT_EQ(faceNormals.size(), 6);
}

}  // namespace
