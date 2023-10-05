#pragma once

#include <string>

#include "starlight/core/Core.h"

namespace sl {

enum class ShaderScope : u8 {
    global   = 0,  // updated once per frame
    instance = 1,  // per instance
    local    = 2   // per object
};

ShaderScope shaderScopeFromString(const std::string& name);
std::string shaderScopeToString(ShaderScope scope);

}  // namespace sl
