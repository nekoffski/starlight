#pragma once

#include <string>
#include <vector>

#include "starlight/core/Core.h"
#include "starlight/core/utils/FileSystem.h"
#include "starlight/renderer/Shader.h"

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

    ShaderStage::Type stage;
    std::string filename;
    std::string source;
};

struct ShaderConfig {
    inline static const std::string baseShaderPath =
      SL_ASSETS_PATH + std::string("/shaders");

    static std::optional<ShaderConfig> load(
      const std::string& name, const std::string& shadersPath = baseShaderPath,
      const FileSystem& fs = fileSystem
    );

    std::string toString() const;

    std::string name;
    bool useInstances;
    bool useLocal;

    std::vector<ShaderAttributeConfig> attributes;
    std::vector<ShaderUniformConfig> uniforms;

    std::string renderpassName;

    std::vector<ShaderStageConfig> stages;
};

}  // namespace sl
