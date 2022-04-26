#include <gtest/gtest.h>

#include "mocks/TestComponent.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/ecs/Registry.hpp"

using namespace testing;
using namespace sl::ecs;

namespace {

struct RegistryTests : Test {
    Registry reg;
    const std::string name = "some-name";

    static inline constexpr int x   = 1;
    static inline constexpr float y = 1.0f;
};

TEST_F(RegistryTests, givenRegistry_whenCreatingEntity_shouldCreateEntity) {
    auto& entity = reg.createEntity(name);

    EXPECT_TRUE(reg.hasEntityById(entity.getId()));
    EXPECT_TRUE(reg.hasEntityByName(name));
}

TEST_F(RegistryTests, givenRegistry_whenAddingComponent_shouldAddComponent) {
    auto& entity = reg.createEntity(name);
    auto id      = entity.getId();

    ASSERT_FALSE(reg.hasComponent<TestComponent>(id));

    reg.addComponent<TestComponent>(id, x, y);
    EXPECT_TRUE(reg.hasComponent<TestComponent>(id));

    auto& component  = reg.getComponent<TestComponent>(id);
    auto& component2 = static_cast<TestComponent&>(
        reg.getComponentByIndex(id, sl::core::typeIndex<TestComponent>())
    );

    EXPECT_TRUE(component.x == component2.x == x);
    EXPECT_TRUE(component.y == component2.y == y);
}

TEST_F(RegistryTests, givenRegistry_whenRemovingAddedomponent_shouldRemoveComponent) {
    auto& entity = reg.createEntity(name);
    auto id      = entity.getId();

    reg.addComponent<TestComponent>(id, x, y);
    ASSERT_TRUE(reg.hasComponent<TestComponent>(id));

    reg.removeComponent(id, sl::core::typeIndex<TestComponent>());

    EXPECT_FALSE(reg.hasComponent<TestComponent>(id));
}

TEST_F(RegistryTests, givenRegistry_whenRemovingEntity_shouldRemoveEntityAndItsComponents) {
    auto& entity = reg.createEntity(name);
    auto id      = entity.getId();

    ASSERT_TRUE(reg.hasEntityById(id));
    ASSERT_TRUE(reg.hasEntityByName(name));
    reg.addComponent<TestComponent>(id, x, y);

    ASSERT_TRUE(reg.hasComponent<TestComponent>(id));

    reg.removeEntity(id);

    EXPECT_FALSE(reg.hasEntityById(id));
    EXPECT_FALSE(reg.hasEntityByName(name));

    EXPECT_FALSE(reg.hasComponent<TestComponent>(id));
}

TEST_F(RegistryTests, givenRegistry_whenClearing_shouldRemoveEntityAndItsComponents) {
    auto& entity = reg.createEntity(name);
    auto id      = entity.getId();

    ASSERT_TRUE(reg.hasEntityById(id));
    ASSERT_TRUE(reg.hasEntityByName(name));
    reg.addComponent<TestComponent>(id, x, y);

    ASSERT_TRUE(reg.hasComponent<TestComponent>(id));

    reg.clear();

    EXPECT_FALSE(reg.hasEntityById(id));
    EXPECT_FALSE(reg.hasEntityByName(name));

    EXPECT_FALSE(reg.hasComponent<TestComponent>(id));
}

TEST_F(RegistryTests, givenRegistry_whenGettingEntitiesCount_shouldReturnCorrectValue) {
    EXPECT_EQ(reg.getEntitiesCount(), 0);

    reg.createEntity("name1");
    EXPECT_EQ(reg.getEntitiesCount(), 1);

    auto& entity = reg.createEntity("name2");
    EXPECT_EQ(reg.getEntitiesCount(), 2);

    reg.removeEntity(entity.getId());
    EXPECT_EQ(reg.getEntitiesCount(), 1);
}

}  // namespace
