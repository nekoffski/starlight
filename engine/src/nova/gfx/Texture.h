#pragma once

#include "nova/core/Memory.hpp"

namespace nova::gfx {

// TODO: implement some kind of CRTP for ids
struct Texture {
    enum class Use { unknown, diffuseMap };

    virtual ~Texture() {}

    uint32_t id = idGenerator++;

    struct Properties {
        uint32_t width;
        uint32_t height;
        uint8_t channels;
        bool isTransparent;
    } props;

    std::string name;
    uint32_t generation;

    inline static uint32_t idGenerator = 0;
};

}  // namespace nova::gfx
