#include "ShaderScope.h"

#include "starlight/core/utils/Log.h"

namespace sl {

ShaderScope shaderScopeFromString(const std::string& name) {
    if (name == "local")
        return ShaderScope::local;
    else if (name == "instance")
        return ShaderScope::instance;
    else if (name == "global")
        return ShaderScope::global;
    FAIL("Could not parse shader scope: {}", name);
}

std::string shaderScopeToString(ShaderScope scope) {
    switch (scope) {
        case ShaderScope::global:
            return "global";
        case ShaderScope::instance:
            return "instance";
        case ShaderScope::local:
            return "local";
    }
    __builtin_unreachable();
}

}  // namespace sl