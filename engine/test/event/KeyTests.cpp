#include "nova/event/Key.h"

#include <vector>

#include <gtest/gtest.h>

using namespace nova::event;
using namespace testing;

using KeyActionTestsParam = std::pair<KeyEvent::Action, std::string>;

struct KeyActionTests : TestWithParam<KeyActionTestsParam> {};

TEST_P(KeyActionTests, givenKeyActionEnum_whenParsingToString_shouldReturnCorrectString) {
    auto [action, str] = GetParam();

    EXPECT_EQ(toString(action), str);
}

static std::vector<KeyActionTestsParam> keyActionsTestsInput = {
    {KeyEvent::Action::press,   "KeyPress"        },
    {KeyEvent::Action::release, "KeyRelease"      },
    {KeyEvent::Action::repeat,  "KeyRepeat"       },
    {KeyEvent::Action::unknown, "KeyUnknownAction"}
};

INSTANTIATE_TEST_SUITE_P(KeyTests, KeyActionTests, ValuesIn(keyActionsTestsInput));
