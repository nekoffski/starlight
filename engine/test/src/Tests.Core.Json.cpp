#include "starlight/core/Json.hh"

#include <gtest/gtest.h>

#include "starlight/math/Json.hh"

using namespace sl;

TEST(JsonTests, getIfExistExistingField) {
    int v = 1;
    nlohmann::json root;
    root["foo"] = 1337;

    json::getIfExists(root, "bar", v);
    EXPECT_EQ(v, 1);

    json::getIfExists(root, "foo", v);
    EXPECT_EQ(v, 1337);
}

TEST(JsonTests, checkIfHasField) {
    nlohmann::json root;
    root["foo"] = 1;
    EXPECT_TRUE(json::hasField(root, "foo"));
    EXPECT_FALSE(json::hasField(root, "bar"));
}

TEST(JsonTests, serializeVec2f32) {
    Vec2<f32> v = { -1.0f, 2.0f };
    nlohmann::json root;
    root["v"] = v;
    auto v2   = root.at("v").get<Vec2<f32>>();
    ASSERT_EQ(v, v2);
}

TEST(JsonTests, serializeVec3f32) {
    Vec3<f32> v = { -0.01234f, 2.0f, 3.5f };
    nlohmann::json root;
    root["v"] = v;
    auto v2   = root.at("v").get<Vec3<f32>>();
    ASSERT_EQ(v, v2);
}

TEST(JsonTests, serializeVec4f32) {
    Vec4<f32> v = { 1.0f, 2.0f, 3.5f, 4.5f };
    nlohmann::json root;
    root["v"] = v;
    auto v2   = root.at("v").get<Vec4<f32>>();
    ASSERT_EQ(v, v2);
}
