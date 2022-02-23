#include <gtest/gtest.h>

#include "mocks/TestComponent.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/ComponentView.hpp"

using namespace testing;
using namespace sl::ecs;

namespace {

struct ComponentViewTests : Test {
    ComponentContainer<TestComponent> container;
    const std::string id = "some-id";
};

TEST_F(ComponentViewTests, givenEmptyView_whenAccessingSize_shouldReturnZero) {
    auto view = ComponentView<TestComponent>{container};

    EXPECT_EQ(view.size(), 0);

    int iterations = 0;

    for (auto& _ : view) ++iterations;

    EXPECT_EQ(iterations, 0);
}

TEST_F(ComponentViewTests, givenEmptyView_whenCheckingIfEntityOwnComponent_shouldReturnFalse) {
    auto view = ComponentView<TestComponent>{container};

    EXPECT_FALSE(view.doesEntityOwnComponent(id));
}

TEST_F(ComponentViewTests,
       givenViewWithFilledContainer_whenCheckingIfEntityOwnComponent_shouldReturnTrue) {
    container.add(id, 1, 1.0f);
    auto view = ComponentView<TestComponent>{container};

    EXPECT_TRUE(view.doesEntityOwnComponent(id));
}

TEST_F(ComponentViewTests, givenViewWithFilledContainer_shouldAllowToAccessComponnet) {
    container.add(id, 1, 1.0f);
    auto view = ComponentView<TestComponent>{container};

    int iterations = 0;

    for (auto& _ : view) ++iterations;

    EXPECT_EQ(iterations, 1);
    EXPECT_EQ(view.size(), 1);

    auto& component = view[0];

    EXPECT_EQ(component.x, 1);
    EXPECT_EQ(component.y, 1.0f);

    auto& component2 = view.getByEntityId(id);

    EXPECT_EQ(component.ownerEntityId, component2.ownerEntityId);
}

}  // namespace
