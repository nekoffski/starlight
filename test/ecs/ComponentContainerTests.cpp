#include "sl/ecs/ComponentContainer.hpp"

#include <gtest/gtest.h>

#include "sl/ecs/Component.h"
#include "mocks/TestComponent.h"

using namespace testing;
using namespace sl::ecs;

namespace {

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

TEST_F(ComponentContainerTests, givenNonEmptyContainer_whenGettingAll_shouldReturnNonVector) {
    m_container.add(m_entityId, m_x, m_y);
    EXPECT_EQ(m_container.getAll().size(), 1);

    m_container.add(m_entityId, m_x, m_y);
    EXPECT_EQ(m_container.getAll().size(), 2);
}

TEST_F(ComponentContainerTests, whenGettingComponentById_shouldReturnCorrectComponent) {
    m_container.add(m_entityId, m_x, m_y);

    auto& component = m_container.getByEntityId(m_entityId);

    EXPECT_EQ(component.x, m_x);
    EXPECT_EQ(component.y, m_y);
    EXPECT_EQ(component.ownerEntityId, m_entityId);
}

TEST_F(ComponentContainerTests, whenRemovingComponent_shouldRemoveComponent) {
    m_container.add(m_entityId, m_x, m_y);
    EXPECT_EQ(m_container.size(), 1);
    EXPECT_TRUE(m_container.doesEntityOwnComponent(m_entityId));

    m_container.remove(m_entityId);
    EXPECT_EQ(m_container.size(), 0);
    EXPECT_FALSE(m_container.doesEntityOwnComponent(m_entityId));
}

TEST_F(ComponentContainerTests, givenContainerWith3Elements_whenRemovingComponent_shouldNotBrokeOtherComponents) {
    m_container.add(m_entityId + "1", m_x + 1, m_y + 1.0f);
    m_container.add(m_entityId + "2", m_x + 2, m_y + 2.0f);
    m_container.add(m_entityId + "3", m_x + 3, m_y + 3.0f);

    auto components = m_container.getAll();

    EXPECT_EQ(components[0].x, m_x + 1);
    EXPECT_EQ(components[0].y, m_y + 1.0f);

    EXPECT_EQ(components[1].x, m_x + 2);
    EXPECT_EQ(components[1].y, m_y + 2.0f);

    EXPECT_EQ(components[2].x, m_x + 3);
    EXPECT_EQ(components[2].y, m_y + 3.0f);

    auto c1 = m_container.getByEntityId(m_entityId + "1");
    EXPECT_EQ(components[0].x, m_x + 1);
    EXPECT_EQ(components[0].y, m_y + 1.0f);

    auto c2 = m_container.getByEntityId(m_entityId + "2");
    EXPECT_EQ(c2.x, m_x + 2);
    EXPECT_EQ(c2.y, m_y + 2.0f);

    auto c3 = m_container.getByEntityId(m_entityId + "3");
    EXPECT_EQ(c3.x, m_x + 3);
    EXPECT_EQ(c3.y, m_y + 3.0f);

    m_container.remove(m_entityId + "2");

    components = m_container.getAll();

    EXPECT_EQ(components[0].x, m_x + 1);
    EXPECT_EQ(components[0].y, m_y + 1.0f);

    EXPECT_EQ(components[1].x, m_x + 3);
    EXPECT_EQ(components[1].y, m_y + 3.0f);

    c1 = m_container.getByEntityId(m_entityId + "1");
    EXPECT_EQ(components[0].x, m_x + 1);
    EXPECT_EQ(components[0].y, m_y + 1.0f);

    c3 = m_container.getByEntityId(m_entityId + "3");
    EXPECT_EQ(c3.x, m_x + 3);
    EXPECT_EQ(c3.y, m_y + 3.0f);
}
}
