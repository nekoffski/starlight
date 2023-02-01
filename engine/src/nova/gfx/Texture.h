#pragma once

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

struct TextureLoader {
    virtual Texture* load(
        const std::string& name, const Texture::Properties& props, const void* pixels
    ) const = 0;

    virtual Texture* load(const std::string& name, const std::string& path) const = 0;
};

}  // namespace nova::gfx
