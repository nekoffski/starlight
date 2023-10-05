#pragma once

#include <string>
#include <vector>

#include "starlight/core/Core.h"
#include "starlight/renderer/Shader.h"
#include "starlight/renderer/ShaderAttribute.h"
#include "starlight/renderer/ShaderScope.h"
#include "starlight/renderer/ShaderUniform.h"

namespace sl {

struct ShaderAttributeConfig {
    std::string toString() const;

    std::string name;
    ShaderAttribute::Type type;
    u8 size;
};

struct ShaderUniformConfig {
    std::string toString() const;

    std::string name;
    u8 size;
    u32 location;
    ShaderUniform::Type type;
    ShaderScope scope;
};

struct ShaderStageConfig {
    std::string toString() const;

    Shader::Stage stage;
    std::string filename;
};

struct ShaderConfig {
    std::string toString() const;

    std::string name;
    bool useInstance;
    bool useLocal;

    std::vector<ShaderAttributeConfig> attributes;
    std::vector<ShaderUniformConfig> uniforms;

    std::string renderpassName;

    std::vector<ShaderStageConfig> stages;
};

}  // namespace sl
