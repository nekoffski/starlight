#include <gtest/gtest.h>

#include "sl/core/Misc.hpp"

TEST(MiscTests, givenType_typeIndexShouldBeEqualToMiscWrapper) {
    using Type = int;

    ASSERT_EQ(sl::core::typeIndex<Type>(), std::type_index(typeid(Type)));
}
