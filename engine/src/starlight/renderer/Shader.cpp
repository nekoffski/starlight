#include "Shader.h"

namespace sl {

Shader::Stage Shader::stageFromString(const std::string& name) {
    if (name == "vertex")
        return stageVertex;
    else if (name == "geometry")
        return stageGeometry;
    else if (name == "fragment")
        return stageFragment;
    else if (name == "compute")
        return stageCompute;
    FAIL("Invalid stage name: {}", name);
}

std::string Shader::stageToString(Shader::Stage stage) {
    switch (stage) {
        case stageVertex:
            return "vertex";
        case stageGeometry:
            return "geometry";
        case stageFragment:
            return "fragment";
        case stageCompute:
            return "compute";
    }
    FAIL("Invalid stage: {}", stage);
}

u16 Shader::getUniformIndex(const std::string& uniformName) { return u16(); }

}  // namespace sl
