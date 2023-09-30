#pragma once

#include "starlight/core/Core.h"

#include "ShaderScope.h"

namespace sl {

struct ShaderUniform {
    enum class Type : u8 {
        float32,
        float32_2,
        float32_3,
        float32_4,
        int8,
        uint8,
        int16,
        uint16,
        int32,
        uint32,
        mat4,
        sampler,
        custom
    };

    u64 offset;
    u16 location;
    u16 index;
    u16 size;
    u8 setIndex;

    ShaderScope scope;
    Type type;
};

}  // namespace sl