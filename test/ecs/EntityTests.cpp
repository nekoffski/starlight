#include <gtest/gtest.h>

#include "mocks/TestComponent.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/ecs/Registry.hpp"

using namespace testing;
using namespace sl::ecs;

namespace {

struct EntityTests : Test {
    Registry reg;
    const std::string name = "name";
    Entity& entity         = reg.createEntity(name);
};

TEST_F(EntityTests, givenEntity_shouldBeActiveAsDefault) { EXPECT_TRUE(entity.isActive); }

TEST_F(EntityTests, givenEntity_whenGettingName_shouldReturnCorrectName) {
    EXPECT_EQ(entity.getName(), name);
}

TEST_F(EntityTests, givenEntity_whenSettingName_shouldSetName) {
    const std::string newName = "new-name";
    ASSERT_NE(entity.getName(), newName);

    entity.setName(newName);
    EXPECT_EQ(entity.getName(), newName);
}

TEST_F(EntityTests, givenEntity_whenAddingEntity_shouldAddEntity) {
    ASSERT_FALSE(entity.hasComponent<TestComponent>());

    constexpr int x   = 1;
    constexpr float y = 1.0f;

    auto& component = entity.addComponent<TestComponent>(x, y);

    EXPECT_EQ(component.x, x);
    EXPECT_EQ(component.y, y);

    ASSERT_TRUE(entity.hasComponent<TestComponent>());

    entity.removeComponent<TestComponent>();

    EXPECT_FALSE(entity.hasComponent<TestComponent>());
}
}  // namespace
