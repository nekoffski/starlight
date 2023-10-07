#pragma once

#include <string>

#include "starlight/core/Core.h"

namespace sl {

struct ShaderStage {
    using Type = u16;

    static constexpr Type vertex   = 0x1;
    static constexpr Type geometry = 0x2;
    static constexpr Type fragment = 0x4;
    static constexpr Type compute  = 0x8;

    static Type typeFromString(const std::string& name);
    static std::string typeToString(Type stage);

    Type type;
    std::string source;
};

}  // namespace sl
