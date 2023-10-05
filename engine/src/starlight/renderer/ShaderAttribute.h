#pragma once

#include <string>

#include "starlight/core/Core.h"

namespace sl {

struct ShaderAttribute {
    enum class Type : u8 {
        float32,
        float32_2,
        float32_3,
        float32_4,
        mat4,
        int8,
        uint8,
        int16,
        uint16,
        int32,
        uint32
    };

    static Type typeFromString(const std::string& name);
    static std::string typeToString(Type type);
    static u32 getTypeSize(Type type);

    std::string name;
    Type type;
    u32 size;
};

}  // namespace sl