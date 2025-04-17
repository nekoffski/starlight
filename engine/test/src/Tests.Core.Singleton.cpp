#include <gtest/gtest.h>

#include "starlight/core/Singleton.hh"

class Singleton : public sl::Singleton<Singleton> {};

TEST(SingletonTests, singletonInstanceAddress) {
    Singleton singleton;
    EXPECT_EQ(&singleton, &Singleton::get());
    EXPECT_EQ(&Singleton::get(), Singleton::getPtr());
}

TEST(SingletonTests, twoInstances) {
    Singleton singleton;
    ASSERT_DEATH(Singleton{}, "");
}

TEST(SingletonTests, noInstance) { EXPECT_EQ(Singleton::getPtr(), nullptr); }

TEST(SingletonTests, getWithoutInstance) { ASSERT_DEATH(Singleton::get(), ""); }

TEST(SingletonTests, isCreated) {
    ASSERT_FALSE(Singleton::isCreated());
    Singleton singleton;
    EXPECT_TRUE(Singleton::isCreated());
}

TEST(SingletonTests, expectCreated) {
    ASSERT_DEATH(sl::expectCreated<Singleton>(), "");
    Singleton singleton;
    EXPECT_NO_FATAL_FAILURE(sl::expectCreated<Singleton>());
}

TEST(SingletonTests, singletonGuard) {
    ASSERT_DEATH(sl::SingletonGuard<Singleton>{}, "");
    Singleton singleton;
    EXPECT_NO_FATAL_FAILURE(sl::SingletonGuard<Singleton>{});
}
