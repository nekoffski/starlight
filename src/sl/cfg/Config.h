#pragma once

#include <string>

namespace sl::cfg {

struct Config {
    struct Paths {
        std::string shaders;
        std::string textures;
        std::string models;
        std::string cubemaps;
    };

    struct Window {
        std::string name = "Starlight editor";
        int width        = 1600;
        int height       = 900;
    };

    Paths paths;
    Window window;
};
}  // namespace sl::cfg