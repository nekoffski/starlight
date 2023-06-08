#include "starlight/core/Input.hpp"

#include <vector>

#include <gtest/gtest.h>

using namespace sl::core;
using namespace testing;

using KeyActionTestsParam = std::pair<KeyAction, std::string>;

struct KeyActionTests : TestWithParam<KeyActionTestsParam> {};

TEST_P(KeyActionTests, givenKeyActionEnum_whenParsingToString_shouldReturnCorrectString) {
    auto [action, str] = GetParam();

    EXPECT_EQ(toString(action), str);
}

static std::vector<KeyActionTestsParam> keyActionsTestsInput = {
    {KeyAction::press,   "KeyPress"        },
    {KeyAction::release, "KeyRelease"      },
    {KeyAction::repeat,  "KeyRepeat"       },
    {KeyAction::unknown, "KeyUnknownAction"}
};

INSTANTIATE_TEST_SUITE_P(KeyTests, KeyActionTests, ValuesIn(keyActionsTestsInput));
