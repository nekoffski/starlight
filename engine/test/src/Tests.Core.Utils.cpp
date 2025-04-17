#include <gtest/gtest.h>

#include "starlight/core/Utils.hh"

struct UtilsTests : ::testing::Test {
    std::vector<int> elements          = { 1, 2, 3, 4 };
    std::vector<const char*> elements2 = { "aaa", "bbb", "ccc" };
};

TEST_F(UtilsTests, containsExisting) { EXPECT_TRUE(sl::contains(elements, 1)); }

TEST_F(UtilsTests, containsNonExisting) {
    EXPECT_FALSE(sl::contains(elements, 1337));
}

TEST_F(UtilsTests, constainsConstCharPtr) {
    std::string text = "bbb";
    EXPECT_TRUE(sl::contains(elements2, text.c_str()));
}

TEST_F(UtilsTests, containtConstCharPtrNonExisting) {
    std::string text = "ddd";
    EXPECT_FALSE(sl::contains(elements2, text.c_str()));
}
