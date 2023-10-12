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
#include "ShaderStage.h"

namespace sl {

class Shader {
public:
    struct Impl {
        virtual ~Impl() = default;

        virtual void initialize() = 0;
        virtual void use()        = 0;
    };

    enum class State : u8 { notCreated, uninitialized, initialized };

    u16 getUniformIndex(const std::string& uniformName);

    Id32 id;
    std::string name;

    bool useInstances;
    bool useLocals;

    u64 requiredUboAlignment = 0;

    u64 globalUboSize   = 0;
    u64 globalUboStride = 0;
    u64 globalUboOffset = 0;

    u64 uboSize   = 0;
    u64 uboStride = 0;

    u64 pushConstantSize   = 0;
    u64 pushConstantStride = 0;

    std::vector<Texture*> globalTextures;
    u8 instanceTextureCount = 0;

    ShaderScope boundScope;

    Id32 boundInstanceId;
    u32 boundUboOffset = 0;

    std::unordered_map<std::string, ShaderUniform> uniforms;
    std::vector<ShaderAttribute> attributes;
    std::vector<ShaderStage> stages;

    State state;

    u8 pushConstantRangeCount = 0;
    std::array<Range, 32> pushConstantRanges;

    u16 attributeStride = 0;

    std::unique_ptr<Impl> impl;

    u32 renderPassId = 0;
};

}  // namespace sl
