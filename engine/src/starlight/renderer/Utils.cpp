#include "CullMode.hh"

#include "starlight/core/Log.hh"

namespace sl {

std::string cullModeToString(CullMode mode) {
    switch (mode) {
        case CullMode::none:
            return "none";
        case CullMode::front:
            return "front";
        case CullMode::back:
            return "back";
        case CullMode::frontAndBack:
            return "frontAndBack";
    }
    FATAL_ERROR("Invalid cull mode");
}

CullMode cullModeFromString(const std::string& mode) {
    if (mode == "none")
        return CullMode::none;
    else if (mode == "front")
        return CullMode::front;
    else if (mode == "back")
        return CullMode::back;
    else if (mode == "frontAndBack")
        return CullMode::frontAndBack;
    FATAL_ERROR("Could not parse cull mode: {}", mode);
}

}  // namespace sl