#pragma once

namespace sl::utils::globals {
struct Config {
    struct Paths {
        std::string shaders;
        std::string textures;
        std::string models;
    };

    Paths paths;
};
}