#include <gtest/gtest.h>

#include "mocks/TestComponent.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/ComponentContainer.hpp"

using namespace testing;
using namespace sl::ecs;

namespace {

struct ComponentContainerTests : public Test {
    ComponentContainer<TestComponent> componentContainer;
    const std::string id = "abcdefgh";
};

TEST_F(ComponentContainerTests,
       givenContainerWithDefaultCtor_whenAccessingCapacity_shouldReturnDefault) {
    EXPECT_EQ(ComponentContainer<TestComponent>{}.capacity(),
              ComponentContainer<TestComponent>::defaultCapacity);
}

TEST_F(ComponentContainerTests,
       givenContainerWithGivenCapacity_whenAccessingCapacity_shouldReturnCorrectValue) {
    constexpr int capacity = 123556;
    EXPECT_EQ(ComponentContainer<TestComponent>{capacity}.capacity(), capacity);
}

TEST_F(ComponentContainerTests, givenEmptyContainer_whenAccessingSize_shouldReturnZero) {
    EXPECT_EQ(componentContainer.size(), 0);
}

TEST_F(ComponentContainerTests,
       givenContainerWithAddedComponent_whenAccessingSize_shouldReturnOne) {
    componentContainer.add(id, 1, 5.0f);

    EXPECT_EQ(componentContainer.size(), 1);
}

TEST_F(ComponentContainerTests,
       givenContainerWithAddedComponent_whenAccessingAllComponents_shouldReturnCorrectComponent) {
    constexpr int x = 1;
    constexpr float y = 5.0f;

    componentContainer.add(id, x, y);

    auto& components = componentContainer.getAll();
    ASSERT_EQ(components.size(), 1);

    auto& component = components.back();

    EXPECT_EQ(x, component.x);
    EXPECT_EQ(y, component.y);
}

TEST_F(ComponentContainerTests,
       givenContainerWithAddedComponent_whenAccessingById_shouldReturnCorrectComponent) {
    constexpr int x = 1;
    constexpr float y = 5.0f;

    componentContainer.add(id, x, y);

    ASSERT_TRUE(componentContainer.doesEntityOwnComponent(id));

    auto& component = componentContainer.getByEntityId(id);

    EXPECT_EQ(x, component.x);
    EXPECT_EQ(y, component.y);
}

TEST_F(ComponentContainerTests, givenContainer_whenAddingComponent_shouldReturnCorrectComponent) {
    constexpr int x = 1;
    constexpr float y = 5.0f;

    auto& component = componentContainer.add(id, x, y);

    EXPECT_EQ(x, component.x);
    EXPECT_EQ(y, component.y);
}

TEST_F(ComponentContainerTests,
       givenContainer_whenRemovingComponet_shouldRemoveComponentAndDoNotAffectOthers) {
    auto id2 = id + "2";

    componentContainer.add(id, 1, 1.0f);
    componentContainer.add(id2, 1, 1.0f);

    ASSERT_TRUE(componentContainer.doesEntityOwnComponent(id));
    ASSERT_TRUE(componentContainer.doesEntityOwnComponent(id2));

    componentContainer.remove(id);

    EXPECT_TRUE(componentContainer.doesEntityOwnComponent(id2));
    EXPECT_FALSE(componentContainer.doesEntityOwnComponent(id));
}

}  // namespace
