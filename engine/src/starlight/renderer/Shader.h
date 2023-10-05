#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <memory>

#include "starlight/core/Id.hpp"
#include "starlight/core/Core.h"
#include "starlight/core/utils/Log.h"

#include "Texture.h"
#include "ShaderScope.h"
#include "ShaderUniform.h"
#include "ShaderAttribute.h"

namespace sl {

class Shader {
    struct Impl {};

public:
    using Stage = u16;

    static constexpr Stage stageVertex   = 0x1;
    static constexpr Stage stageGeometry = 0x2;
    static constexpr Stage stageFragment = 0x4;
    static constexpr Stage stageCompute  = 0x8;

    static Stage stageFromString(const std::string& name);
    static std::string stageToString(Stage stage);

    enum class State : u8 { notCreated, uninitialized, initialized };

    u16 getUniformIndex(const std::string& uniformName);

    Id32 id;
    std::string name;

    bool useInstances;
    bool useLocals;

    u64 requiredUboAlignment;

    u64 globalUboSize;
    u64 globalUboStride;
    u64 globalUboOffset;

    u64 uboSize;
    u64 uboStride;

    u64 pushConstantSize;
    u64 pushConstantStride;

    std::vector<Texture*> globalTextures;
    u8 instanceTextureCount;

    ShaderScope boundScope;

    Id32 boundInstanceId;
    u32 boundUboOffset;

    std::unordered_map<std::string, ShaderUniform> uniforms;
    std::vector<ShaderAttribute> attributes;

    State state;

    u8 pushConstantRangeCount;
    std::array<Range, 32> pushConstantRanges;

    u16 attributeStride;

    std::unique_ptr<Impl> impl;
};

}  // namespace sl
