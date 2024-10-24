#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <memory>
#include <functional>
#include <concepts>
#include <span>
#include <string_view>

#include "starlight/core/math/Core.hh"
#include "starlight/core/memory/Memory.hh"
#include "starlight/core/utils/Id.hh"
#include "starlight/core/FileSystem.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/utils/Resource.hh"
#include "starlight/core/Log.hh"

#include "starlight/renderer/CullMode.hh"
#include "starlight/renderer/PolygonMode.hh"
#include "starlight/renderer/gpu/RendererBackend.hh"

#include "Texture.hh"

namespace sl {

template <typename T>
concept GlmCompatible = requires(T object) {
    { glm::value_ptr(object) } -> std::convertible_to<void*>;
};

class Shader : public NonMovable, public Identificable<Shader> {
    inline static const std::string baseShadersPath =
      SL_ASSETS_PATH + std::string("/shaders");

public:
    enum class Scope : u8 {
        global   = 0,  // updated once per frame
        instance = 1,  // per instance
        local    = 2   // per object
    };

    static Scope scopeFromString(const std::string& name);
    static std::string scopeToString(Scope scope);

    static ResourceRef<Shader> load(
      const std::string& name, std::string_view shadersPath = baseShadersPath,
      const FileSystem& fs = fileSystem
    );

    static ResourceRef<Shader> find(const std::string& name);
    static ResourceRef<Shader> find(const u64 id);

    struct Attribute {
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
        PolygonMode polygonMode;
    };

    class UniformProxy {
        friend class Shader;

    public:
        void set(const std::string& uniform, const Texture* value);

        template <typename T>
        requires GlmCompatible<T>
        void set(const std::string& uniform, const T& value) {
            m_shader.setUniform(uniform, glm::value_ptr(value), m_commandBuffer);
        }

        template <typename T> void set(const std::string& uniform, const T* value) {
            m_shader.setUniform(uniform, value, m_commandBuffer);
        }

        template <typename T> void set(const std::string& uniform, const T& value) {
            m_shader.setUniform(uniform, &value, m_commandBuffer);
        }

    private:
        explicit UniformProxy(Shader& shader, CommandBuffer& commandBuffer);
        Shader& m_shader;
        CommandBuffer& m_commandBuffer;
    };

    using UniformCallback = std::function<void(UniformProxy&)>;

    virtual ~Shader() = default;

    virtual void use(CommandBuffer&)                                            = 0;
    virtual u32 acquireInstanceResources(const std::vector<Texture*>& textures) = 0;
    virtual void releaseInstanceResources(u32 instanceId)                       = 0;

    virtual void createPipeline(RenderPass& renderPass) = 0;

    void setGlobalUniforms(
      CommandBuffer& commandBuffer, u32 imageIndex, UniformCallback&& callback
    );
    void setInstanceUniforms(
      CommandBuffer& commandBuffer, u32 instanceId, u32 imageIndex,
      UniformCallback&& callback
    );
    void setLocalUniforms(CommandBuffer& commandBuffer, UniformCallback&& callback);

    const std::string& getName() const;

protected:
    explicit Shader(const Properties& props);

    std::string m_name;
    bool m_useInstances;
    bool m_useLocals;

    CullMode m_cullMode;
    PolygonMode m_polygonMode;

private:
    virtual void bindGlobals()                                               = 0;
    virtual void bindInstance(u32 instanceId)                                = 0;
    virtual void applyGlobals(CommandBuffer& commandBuffer, u32 imageIndex)  = 0;
    virtual void applyInstance(CommandBuffer& commandBuffer, u32 imageIndex) = 0;

    virtual void setSampler(const std::string& uniform, const Texture* value) = 0;
    virtual void setUniform(
      const std::string& name, const void* value, CommandBuffer& commandBuffer
    ) = 0;
};

class ShaderManager
    : public ResourceManager<Shader>,
      public kc::core::Singleton<ShaderManager> {
public:
    explicit ShaderManager(RendererBackend& renderer);

    ResourceRef<Shader> load(
      const std::string& name, std::string_view shadersPath, const FileSystem& fs
    );

private:
    RendererBackend& m_renderer;
};

}  // namespace sl
