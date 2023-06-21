#pragma once

namespace sl {

struct Config {
    struct Window {
        const int width        = 1600;
        const int height       = 900;
        const std::string name = "Starlight";
    } window;

    struct Version {
        const int major = 0;
        const int minor = 1;
        const int build = 1;
    } version;
};

}  // namespace sl