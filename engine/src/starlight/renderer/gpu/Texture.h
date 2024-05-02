#pragma once

#include <span>

#include "starlight/core/Core.h"
#include "starlight/core/memory/Memory.hpp"

namespace sl {

class Texture {
public:
    enum class Type : u8 { flat, cubemap };
    enum class Use { unknown, diffuseMap, specularMap, normalMap, cubeMap };
    enum class Filter { nearest, linear };
    enum class Repeat { repeat, mirroredRepeat, clampToEdge, clampToBorder };

    struct Properties {
        u32 width;
        u32 height;
        u32 channels;
        bool isTransparent;
        bool isWritable;
        std::string name;
        Type type;
        Use use              = Use::unknown;
        Filter minifyFilter  = Filter::linear;
        Filter magnifyFilter = Filter::linear;
        Repeat uRepeat       = Repeat::repeat;
        Repeat vRepeat       = Repeat::repeat;
        Repeat wRepeat       = Repeat::repeat;
    };

    virtual ~Texture() = default;

    virtual void resize(u32 width, u32 height)           = 0;
    virtual void write(u32 offset, std::span<u8> pixels) = 0;

    const Properties& getProperties() const;
    const u32 getId() const;

    static inline Texture* defaultDiffuse  = nullptr;
    static inline Texture* defaultNormal   = nullptr;
    static inline Texture* defaultSpecular = nullptr;

protected:
    explicit Texture(const Properties& props, u32 id);

    Properties m_props;
    u32 m_id;
};

}  // namespace sl
