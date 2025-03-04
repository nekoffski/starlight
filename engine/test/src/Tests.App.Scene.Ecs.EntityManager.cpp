#include <gtest/gtest.h>

#include "starlight/app/scene2/ecs/EntityManager.hh"

using namespace sl;

struct EntityManagerTests : testing::Test {
    v2::EntityManager entityManager;
    std::string name = "exampleEntity";
};

TEST_F(
  EntityManagerTests,
  givenEntityManager_whenManagingEntitites_shouldReturnCorrectValues
) {
    ASSERT_FALSE(entityManager.hasEntity(name));
    ASSERT_EQ(entityManager.getEntity(name), nullptr);

    auto& entity = entityManager.addEntity(name);
    ASSERT_TRUE(entityManager.hasEntity(name));
    ASSERT_EQ(entityManager.getEntity(name), &entity);
}