#include "starlight/core/FlatMap.hh"

#include <gtest/gtest.h>

#include <array>
#include <concepts>
#include <memory>
#include <string>
#include <utility>

namespace {

using IntMap = sl::FlatMap<int, int>;

static_assert(std::same_as<
              decltype(std::declval<IntMap&>().find(0)),
              int*>);
static_assert(std::same_as<
              decltype(std::declval<const IntMap&>().find(0)),
              const int*>);

struct CopyOnly {
    explicit CopyOnly(int value) : value(value) {}
    CopyOnly(const CopyOnly&) = default;
    CopyOnly(CopyOnly&&) = delete;

    int value;
};

struct ConstructedInPlace {
    ConstructedInPlace(int number, std::string text)
        : number(number), text(std::move(text)) {}

    int number;
    std::string text;
};

TEST(FlatMap, FindsInsertedValues) {
    IntMap map;

    EXPECT_FALSE(map.has(1));
    EXPECT_EQ(map.find(1), nullptr);

    map.insert(1, 42);

    ASSERT_NE(map.find(1), nullptr);
    EXPECT_EQ(*map.find(1), 42);
    EXPECT_TRUE(map.has(1));
}

TEST(FlatMap, SupportsCopyOnlyAndMoveOnlyValues) {
    sl::FlatMap<int, CopyOnly> copied;
    const CopyOnly value{42};
    copied.insert(1, value);

    ASSERT_NE(copied.find(1), nullptr);
    EXPECT_EQ(copied.find(1)->value, 42);

    sl::FlatMap<int, std::unique_ptr<int>> moved;
    auto pointer = std::make_unique<int>(43);
    moved.insert(1, std::move(pointer));

    EXPECT_EQ(pointer, nullptr);
    ASSERT_NE(moved.find(1), nullptr);
    EXPECT_EQ(**moved.find(1), 43);
}

TEST(FlatMap, ConstructsValuesInPlace) {
    sl::FlatMap<int, ConstructedInPlace> map;

    auto& value = map.emplace(1, 42, "answer");

    EXPECT_EQ(value.number, 42);
    EXPECT_EQ(value.text, "answer");
    EXPECT_EQ(map.find(1), &value);
}

TEST(FlatMap, RemovesOneOrSeveralKeys) {
    IntMap map;
    map.insert(1, 10);
    map.insert(2, 20);
    map.insert(3, 30);

    map.remove(1);
    const std::array keys{2, 4};
    map.remove(keys);

    EXPECT_FALSE(map.has(1));
    EXPECT_FALSE(map.has(2));
    EXPECT_TRUE(map.has(3));
}

TEST(FlatMap, VisitsMutableAndConstValues) {
    IntMap map;
    map.insert(1, 10);
    map.insert(2, 20);

    map.forEach([](const int&, int& value) { ++value; });

    const auto& constMap = map;
    int total = 0;
    constMap.forEach(
        [&total](const int&, const int& value) { total += value; }
    );

    EXPECT_EQ(total, 32);
}

}  // namespace
