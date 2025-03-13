#include <gtest/gtest.h>

#include "starlight/app/scene/Scene.hh"

struct SceneTests : ::testing::Test {
    sl::Scene scene;
};

TEST_F(SceneTests, givenScene_whenAddingSameComponenToTwoEntites_shouldWork) {
    auto& entity = scene.addEntity("test-entity");
    entity.addComponent<sl::PointLight>();

    auto& entity2 = scene.addEntity("test-entity2");
    entity2.addComponent<sl::PointLight>();
}

TEST_F(SceneTests, givenScene_whenAddingComponent_shouldHaveCorrectValues) {
    sl::Vec4<sl::f32> color{ 1.0f };
    sl::Vec3<sl::f32> position{ 0.0f };

    auto& entity = scene.addEntity("test-entity");
    entity.addComponent<sl::PointLight>();

    auto& entity2 = scene.addEntity("test-entity2");
    entity2.addComponent<sl::PointLight>();

    auto& c =
      *static_cast<sl::PointLight*>(entity2.getComponent(typeid(sl::PointLight)));

    EXPECT_FLOAT_EQ(c.color.x, color.x);
    EXPECT_FLOAT_EQ(c.color.y, color.y);
    EXPECT_FLOAT_EQ(c.color.z, color.z);
    EXPECT_FLOAT_EQ(c.color.w, color.w);

    EXPECT_FLOAT_EQ(c.position.x, position.x);
    EXPECT_FLOAT_EQ(c.position.y, position.y);
    EXPECT_FLOAT_EQ(c.position.z, position.z);
}
