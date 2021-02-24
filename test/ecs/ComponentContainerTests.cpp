#include "sl/ecs/ComponentContainer.hpp"

#include <gtest/gtest.h>

#include "sl/ecs/Component.h"

using namespace testing;
using namespace sl::ecs;

namespace {

struct TestComponent : public Component {
    explicit TestComponent(int x, float y)
        : x(x)
        , y(y) {}

    int x;
    float y;
};

struct ComponentContainerTests : public Test {
    ComponentContainer<TestComponent> m_container;

    inline static const int m_x = 5;
    inline static const float m_y = 3.0f;
    inline static std::string m_entityId = "abcedfghijk";
};

TEST_F(ComponentContainerTests, givenEmptyContainer_shouldHaveZeroSize) {
    EXPECT_EQ(m_container.size(), 0);
}

TEST_F(ComponentContainerTests, givenContainerWithDefaultCapacity_shouldHaveDefaultCapacity) {
    EXPECT_EQ(m_container.capacity(), ComponentContainer<TestComponent>::defaultCapacity);
}

TEST_F(ComponentContainerTests, givenContainerWithGivenCapacity_shouldHaveCorrectCapacity) {
    constexpr int capacity = 5000;
    EXPECT_EQ(ComponentContainer<TestComponent>{ capacity }.capacity(), capacity);
}

TEST_F(ComponentContainerTests, givenContainer_whenAddingNewComponent_shouldIncreaseSize) {
    m_container.add(m_entityId, m_x, m_y);

    EXPECT_EQ(m_container.size(), 1);
}

TEST_F(ComponentContainerTests, givenContainer_whenAddingNewComponent_shouldEntityBecameOwner) {
    EXPECT_FALSE(m_container.doesEntityOwnComponent(m_entityId));

    m_container.add(m_entityId, m_x, m_y);

    EXPECT_TRUE(m_container.doesEntityOwnComponent(m_entityId));
}

TEST_F(ComponentContainerTests, givenEmptyContainer_whenGettingAll_shouldReturnEmptyVector) {
	EXPECT_EQ(m_container.getAll().size(), 0);
}

}
