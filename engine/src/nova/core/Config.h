#pragma once

#include <string>

#include "math/Size.hpp"

namespace nova::core {

using namespace std::string_literals;

struct Config {
    struct Window {
        math::Size2i size;
        std::string name;
    } window;
};

// clang-format off
const Config defaultConfig = Config{
    .window = {
        .size = { 1600, 900 },
        .name = "Nova Engine"s
    }
};
// clang-format on

}  // namespace nova::core