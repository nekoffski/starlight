#pragma once

namespace nova::gfx {

struct Texture {
    uint32_t id;
    uint32_t width;
    uint32_t height;

    uint8_t channels;
    bool isTransparent;
    uint32_t generation;
};

}  // namespace nova::gfx
