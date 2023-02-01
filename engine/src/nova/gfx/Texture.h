#pragma once

#include "nova/core/Memory.hpp"

namespace nova::gfx {

struct Texture {
    uint32_t id;

    struct Properties {
        uint32_t width;
        uint32_t height;
        uint8_t channels;
        bool isTransparent;
    } props;

    std::string name;
    uint32_t generation;
};

}  // namespace nova::gfx
