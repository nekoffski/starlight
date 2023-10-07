#include "ShaderStage.h"

#include "starlight/core/utils/Log.h"

namespace sl {

ShaderStage::Type ShaderStage::typeFromString(const std::string& name) {
    if (name == "vertex")
        return vertex;
    else if (name == "geometry")
        return geometry;
    else if (name == "fragment")
        return fragment;
    else if (name == "compute")
        return compute;
    FAIL("Invalid stage name: {}", name);
}

std::string ShaderStage::typeToString(ShaderStage::Type type) {
    switch (type) {
        case vertex:
            return "vertex";
        case geometry:
            return "geometry";
        case fragment:
            return "fragment";
        case compute:
            return "compute";
    }
    FAIL("Invalid stage type: {}", type);
}

}  // namespace sl
