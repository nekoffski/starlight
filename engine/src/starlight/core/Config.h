#pragma once

#include <string>

namespace sl {

struct Config {
    struct Window {
        const int width        = 1920;
        const int height       = 1080;
        const std::string name = "Starlight";
    } window;

    struct Version {
        const int major = 0;
        const int minor = 1;
        const int build = 1;
    } version;
};

}  // namespace sl