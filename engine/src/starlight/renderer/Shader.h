#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <memory>
#include <functional>
#include <concepts>

#include "starlight/core/math/Glm.h"
#include "starlight/core/Id.hpp"
#include "starlight/core/Core.h"
#include "starlight/core/utils/Log.h"

#include "Texture.h"
#include "ShaderScope.h"
#include "ShaderUniform.h"
#include "ShaderAttribute.h"
#include "ShaderStage.h"

namespace sl {

template <typename T>
concept GlmCompatible = requires(T object) {
    { glm::value_ptr(object) } -> std::convertible_to<void*>;
};

// TODO: implement higher coupling, is Impl necessary?
class Shader {
public:
    using SelfCallback = std::function<void(Shader*)>;

    struct Impl {
        virtual ~Impl() = default;

        virtual void initialize()                                          = 0;
        virtual void use()                                                 = 0;
        virtual void bindGlobals()                                         = 0;
        virtual void bindInstance(u32 instanceId)                          = 0;
        virtual void applyGlobals()                                        = 0;
        virtual void applyInstance()                                       = 0;
        virtual u32 acquireInstanceResources()                             = 0;
        virtual void releaseInstanceResources(u32 instanceId)              = 0;
        virtual void setUniform(const ShaderUniform& uniform, void* value) = 0;
    };

    void setGlobalUniforms(SelfCallback&&);
    void setInstanceUniforms(u32 instanceId, SelfCallback&&);
    void setLocalUniforms(SelfCallback&&);

    void use();
    void bindGlobals();
    void bindInstance(u32 instanceId);
    void applyGlobals();
    void applyInstance();
    u32 acquireInstanceResources();
    void releaseInstanceResources(u32 instanceId);

    void setUniform(const std::string& uniform, void* value);
    void setUniform(const std::string& uniform, Texture* value);

    void setUniform(const std::string& uniform, GlmCompatible auto& value) {
        setUniform(uniform, glm::value_ptr(value));
    }

    template <typename T>
    requires std::is_arithmetic_v<T>
    void setUniform(const std::string& uniform, T value) {
        setUniform(uniform, &value);
    }

    enum class State : u8 { notCreated, uninitialized, initialized };

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
