#include <gtest/gtest.h>

#include <starlight/core/types/NotNullPtr.hpp>

using namespace starl::core::types;

class NotNullPtrTest : public testing::Test {
protected:
    template<typename T>
    void foo(const NotNullPtr<T>&) {
    }
};

TEST_F(NotNullPtrTest, givenNotNullPtr_whenPassingNullptr_expectThrow) {
    int* emptyPtr = nullptr;

    EXPECT_THROW(foo<int>(emptyPtr), std::logic_error);
}

TEST_F(NotNullPtrTest, givenNotNullPtr_whenPassingValidPtr_expectNoThrow) {
    int x = 5;

    EXPECT_NO_THROW(foo<int>(&x));
}

TEST_F(NotNullPtrTest, givenNotNullPtr_whenGettingValue_expectCorrectValue) {
    int x = 5;
    auto ptr = NotNullPtr<int>(&x);

    EXPECT_EQ(*ptr, x);
}

TEST_F(NotNullPtrTest, givenNotNullPtrToStruct_whenGettingMembers_expectCorrectValues) {
    struct A {
        int x;
        float y;
    };

    auto a = A{1, 1.5f};
    auto ptr = NotNullPtr<A>(&a);

    EXPECT_EQ(a.x, ptr->x);
    EXPECT_EQ(a.y, ptr->y);
}
