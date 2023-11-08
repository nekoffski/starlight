#pragma once

#include "starlight/core/Core.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/math/Size.hpp"

namespace sl {

class Texture {
public:
    enum class Filter { nearest, linear };
    enum class Repeat { repeat, mirroredRepeat, clampToEdge, clmapToBorder };

    struct Properties {
        u32 width;
        u32 height;
        u32 channels;
        bool isTransparent;
        bool isWritable;
        std::string name;
    };

    virtual ~Texture() = default;

    const Properties& getProperties() const;
    const u32 getId() const;

protected:
    explicit Texture(const Properties& props, u32 id);

    Properties m_props;
    u32 m_generation;
    u32 m_id;
};

struct TextureMap {
    enum class Use { unknown, diffuseMap, specularMap, normalMap };

    Texture* texture;
    Use use;
};

}  // namespace sl
