#include "sl/ecs/ComponentMap.hpp"

#include <gtest/gtest.h>

#include "mocks/TestComponent.h"
#include "sl/ecs/Component.h"

using namespace testing;
using namespace sl::ecs;

namespace {

struct ComponentMapTests : public Test {
    ComponentMap componentMap;
};

TEST_F(ComponentMapTests, givenEmptyMap_whenCheckingIfContainerExists_shouldReturnFalse) {
    EXPECT_FALSE(componentMap.exists<TestComponent>());
    EXPECT_FALSE(componentMap.exists<TestComponent2>());
}

TEST_F(ComponentMapTests, givenEmptyMap_whenGettingNotExistingContainer_shouldCreateThatContainer) {
    ASSERT_FALSE(componentMap.exists<TestComponent>());

    componentMap.get<TestComponent>();

    EXPECT_TRUE(componentMap.exists<TestComponent>());
    EXPECT_NO_FATAL_FAILURE({
        componentMap.getByIndex(sl::core::typeIndex<TestComponent>());
    });
}

TEST_F(ComponentMapTests, givenMap_whenClearing_shouldRemoveContainers) {
    ASSERT_FALSE(componentMap.exists<TestComponent>());
    componentMap.get<TestComponent>();
    ASSERT_TRUE(componentMap.exists<TestComponent>());

    componentMap.clear();

    EXPECT_FALSE(componentMap.exists<TestComponent>());
}

}
