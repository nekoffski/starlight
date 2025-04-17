#include <gtest/gtest.h>

#include "starlight/app/scene/Scene.hh"
#include "starlight/app/scene/Components.hh"

struct SceneTests : ::testing::Test {
    sl::Scene scene;
};

TEST_F(SceneTests, addSameComponentTwice) {
    auto& entity = scene.addEntity("test-entity");
    entity.add<sl::PointLightComponent>();

    auto& entity2 = scene.addEntity("test-entity2");
    entity2.add<sl::PointLightComponent>();
}

TEST_F(SceneTests, addComponent) {
    sl::Vec4<sl::f32> color{ 1.0f };
    sl::Vec3<sl::f32> position{ 0.0f };

    auto& entity = scene.addEntity("test-entity");
    entity.add<sl::PointLightComponent>();

    auto& entity2 = scene.addEntity("test-entity2");
    entity2.add<sl::PointLightComponent>();

    auto& c = entity2.get<sl::PointLightComponent>()->data();

    EXPECT_FLOAT_EQ(c.color.x, color.x);
    EXPECT_FLOAT_EQ(c.color.y, color.y);
    EXPECT_FLOAT_EQ(c.color.z, color.z);
    EXPECT_FLOAT_EQ(c.color.w, color.w);

    EXPECT_FLOAT_EQ(c.position.x, position.x);
    EXPECT_FLOAT_EQ(c.position.y, position.y);
    EXPECT_FLOAT_EQ(c.position.z, position.z);
}
