#pragma once

#include <optional>
#include <string>

#include "Core.hh"

namespace sl {

struct Config {
    static std::optional<Config> fromJson(const std::string& path);

    struct Window {
        u32 width;
        u32 height;
        std::string name;
    } window;

    struct Version {
        u32 major;
        u32 minor;
        u32 build;
    } version;

    struct Paths {
        std::string textures;
        std::string shaders;
        std::string materials;
        std::string fonts;
    } paths;
};

}  // namespace sl