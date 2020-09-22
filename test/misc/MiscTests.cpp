#include <gtest/gtest.h>

#include "sl/misc/misc.hpp"

TEST(MiscTests, givenType_typeIndexShouldBeEqualToMiscWrapper) {
    using Type = int;

    ASSERT_EQ(TYPE_INDEX(Type), std::type_index(typeid(Type)));
}
