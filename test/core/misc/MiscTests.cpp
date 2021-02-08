#include <gtest/gtest.h>

#include "sl/core/misc/misc.hpp"

TEST(MiscTests, givenType_typeIndexShouldBeEqualToMiscWrapper) {
    using Type = int;

    ASSERT_EQ(sl::core::misc::typeIndex<Type>(), std::type_index(typeid(Type)));
}
