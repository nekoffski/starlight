#pragma once

#include <string>

#include "starlight/math/Size.hpp"

namespace sl::core {

using namespace std::string_literals;

struct Config {
    struct Window {
        math::Size2i size;
        std::string name;
    } window;

    struct Version {
        int major;
        int minor;
        int build;
    } version;
};

// clang-format off
const Config defaultConfig = Config{
    .window = {
        .size = { 1600, 900 },
        .name = "Nova Engine"s
    },
    .version = {
        .major = 1,
        .minor = 0,
        .build = 0
    }
    
};
// clang-format on

}  // namespace sl::core