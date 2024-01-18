#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <memory>
#include <functional>
#include <concepts>
#include <span>

#include "starlight/core/math/Glm.h"
#include "starlight/core/Id.hpp"
#include "starlight/core/Core.h"
#include "starlight/core/utils/Log.h"
#include "starlight/renderer/CullMode.h"
#include "fwd.h"

#include "Texture.h"

namespace sl {

template <typename T>
concept GlmCompatible = requires(T object) {
    { glm::value_ptr(object) } -> std::convertible_to<void*>;
};

class Shader {
public:
    enum class Scope : u8 {
        global   = 0,  // updated once per frame
        instance = 1,  // per instance
        local    = 2   // per object
    };

    static Scope scopeFromString(const std::string& name);
    static std::string scopeToString(Scope scope);

    struct Attribute {
        // clang-format off
        enum class Type : u8 {
            float32, float32_2, float32_3, float32_4, mat4, int8, uint8,
            int16, uint16, int32, uint32
        };
        // clang-format on

        static Type typeFromString(const std::string& name);
        static std::string typeToString(Type type);
        static u32 getTypeSize(Type type);

        std::string name;
        Type type;
        u32 size;
    };

    struct Stage {
        enum class Type : u8 { vertex, geometry, fragment, compute };

        static Type typeFromString(const std::string& name);
        static std::string typeToString(Type stage);

        Type type;
        std::string source;
    };

    struct Uniform {
        // clang-format off
        enum class Type : u8 {
            float32, float32_2, float32_3, float32_4, int8,
            uint8, int16, uint16, int32, uint32, mat4, sampler, custom
        };
        // clang-format on

        struct Properties {
            std::string name;
            u8 size;
            u32 location;
            Uniform::Type type;
            Scope scope;
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

        Scope scope;
        Type type;
    };

    struct Properties {
        std::string name;
        bool useInstances;
        bool useLocals;

        std::vector<Attribute> attributes;
        std::vector<Stage> stages;
        std::vector<Uniform::Properties> uniformProperties;
        Texture* defaultTexture;

        CullMode cullMode;
    };

    class UniformProxy {
        friend class Shader;

    public:
        void set(const std::string& uniform, TextureMap* value);
        void set(const std::string& uniform, const GlmCompatible auto& value) {
            m_shader.setUniform(uniform, glm::value_ptr(value));
        }

        template <typename T>
        requires std::is_arithmetic_v<T>
        void set(const std::string& uniform, T value) {
            m_shader.setUniform(uniform, &value);
        }

    private:
        explicit UniformProxy(Shader& shader);
        Shader& m_shader;
    };

    using UniformCallback = std::function<void(UniformProxy&)>;

    virtual ~Shader() = default;

    // clang-format off
    virtual void use() = 0;
    virtual u32 acquireInstanceResources(
        const std::vector<TextureMap*>& textureMaps
    ) = 0;
    virtual void releaseInstanceResources(u32 instanceId) = 0;
    // clang-format on

    virtual void createPipeline(RenderPass* renderPass) = 0;

    void setGlobalUniforms(UniformCallback&&);
    void setInstanceUniforms(u32 instanceId, UniformCallback&&);
    void setLocalUniforms(UniformCallback&&);

    u32 getId() const;
    const std::string& getName() const;

protected:
    explicit Shader(const Properties& props, u32 id);

    u32 m_id;
    std::string m_name;
    bool m_useInstances;
    bool m_useLocals;

    UniformProxy m_uniformProxy;

    CullMode m_cullMode;

private:
    virtual void bindGlobals()                                             = 0;
    virtual void bindInstance(u32 instanceId)                              = 0;
    virtual void applyGlobals()                                            = 0;
    virtual void applyInstance()                                           = 0;
    virtual void setUniform(const std::string& uniform, const void* value) = 0;
};

}  // namespace sl
