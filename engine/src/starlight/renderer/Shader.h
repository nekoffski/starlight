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

namespace sl {

template <typename T>
concept GlmCompatible = requires(T object) {
    { glm::value_ptr(object) } -> std::convertible_to<void*>;
};

enum class ShaderScope : u8 {
    global   = 0,  // updated once per frame
    instance = 1,  // per instance
    local    = 2   // per object
};

ShaderScope shaderScopeFromString(const std::string& name);
std::string shaderScopeToString(ShaderScope scope);

struct ShaderAttribute {
    enum class Type : u8 {
        float32,
        float32_2,
        float32_3,
        float32_4,
        mat4,
        int8,
        uint8,
        int16,
        uint16,
        int32,
        uint32
    };

    static Type typeFromString(const std::string& name);
    static std::string typeToString(Type type);
    static u32 getTypeSize(Type type);

    std::string name;
    Type type;
    u32 size;
};

struct ShaderStage {
    using Type = u16;

    static constexpr Type vertex   = 0x1;
    static constexpr Type geometry = 0x2;
    static constexpr Type fragment = 0x4;
    static constexpr Type compute  = 0x8;

    static Type typeFromString(const std::string& name);
    static std::string typeToString(Type stage);

    Type type;
    std::string source;
};

struct ShaderUniform {
    enum class Type : u8 {
        float32,
        float32_2,
        float32_3,
        float32_4,
        int8,
        uint8,
        int16,
        uint16,
        int32,
        uint32,
        mat4,
        sampler,
        custom
    };

    bool isSampler() const;

    static Type typeFromString(const std::string& name);
    static std::string typeToString(Type type);
    static u32 getTypeSize(Type type);

    u64 offset;
    u16 location;
    u16 index;
    u16 size;
    Id8 setIndex;

    ShaderScope scope;
    Type type;
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
