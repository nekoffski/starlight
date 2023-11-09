#include "Shader.h"

namespace sl {

ShaderScope shaderScopeFromString(const std::string& name) {
    if (name == "local")
        return ShaderScope::local;
    else if (name == "instance")
        return ShaderScope::instance;
    else if (name == "global")
        return ShaderScope::global;
    FAIL("Could not parse shader scope: {}", name);
}

std::string shaderScopeToString(ShaderScope scope) {
    switch (scope) {
        case ShaderScope::global:
            return "global";
        case ShaderScope::instance:
            return "instance";
        case ShaderScope::local:
            return "local";
    }
    __builtin_unreachable();
}

ShaderAttribute::Type ShaderAttribute::typeFromString(const std::string& name) {
    static const std::unordered_map<std::string_view, ShaderAttribute::Type>
      nameToType{
          {"f32",   ShaderAttribute::Type::float32  },
          { "vec2", ShaderAttribute::Type::float32_2},
          { "vec3", ShaderAttribute::Type::float32_3},
          { "vec4", ShaderAttribute::Type::float32_4},
          { "u8",   ShaderAttribute::Type::uint8    },
          { "u16",  ShaderAttribute::Type::uint16   },
          { "u32",  ShaderAttribute::Type::uint32   },
          { "i8",   ShaderAttribute::Type::int8     },
          { "i16",  ShaderAttribute::Type::int16    },
          { "i32",  ShaderAttribute::Type::int32    },
    };
    const auto record = nameToType.find(name);
    ASSERT(record != nameToType.end(), "Invalid type attribute name: {}", name);
    return record->second;
}

std::string ShaderAttribute::typeToString(Type type) {
    static const std::unordered_map<ShaderAttribute::Type, std::string> typeToName{
        {ShaderAttribute::Type::float32,    "f32" },
        { ShaderAttribute::Type::float32_2, "vec2"},
        { ShaderAttribute::Type::float32_3, "vec3"},
        { ShaderAttribute::Type::float32_4, "vec4"},
        { ShaderAttribute::Type::uint8,     "u8"  },
        { ShaderAttribute::Type::uint16,    "u16" },
        { ShaderAttribute::Type::uint32,    "u32" },
        { ShaderAttribute::Type::int8,      "i8"  },
        { ShaderAttribute::Type::int16,     "i16" },
        { ShaderAttribute::Type::int32,     "i32" },
    };
    const auto record = typeToName.find(type);
    ASSERT(record != typeToName.end(), "Invalid attribute type: {}", type);
    return record->second;
}

u32 ShaderAttribute::getTypeSize(Type type) {
    static const std::unordered_map<ShaderAttribute::Type, u32> typeToSize{
        {ShaderAttribute::Type::float32,    4 },
        { ShaderAttribute::Type::float32_2, 8 },
        { ShaderAttribute::Type::float32_3, 12},
        { ShaderAttribute::Type::float32_4, 16},
        { ShaderAttribute::Type::uint8,     1 },
        { ShaderAttribute::Type::uint16,    2 },
        { ShaderAttribute::Type::uint32,    4 },
        { ShaderAttribute::Type::int8,      1 },
        { ShaderAttribute::Type::int16,     2 },
        { ShaderAttribute::Type::int32,     4 },
    };
    const auto record = typeToSize.find(type);
    ASSERT(record != typeToSize.end(), "Invalid attribute type: {}", type);
    return record->second;
}

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

bool ShaderUniform::isSampler() const { return type == Type::sampler; }

ShaderUniform::Type ShaderUniform::typeFromString(const std::string& name) {
    static const std::unordered_map<std::string_view, ShaderUniform::Type>
      nameToType{
          {"f32",   ShaderUniform::Type::float32  },
          { "vec2", ShaderUniform::Type::float32_2},
          { "vec3", ShaderUniform::Type::float32_3},
          { "vec4", ShaderUniform::Type::float32_4},
          { "u8",   ShaderUniform::Type::uint8    },
          { "u16",  ShaderUniform::Type::uint16   },
          { "u32",  ShaderUniform::Type::uint32   },
          { "i8",   ShaderUniform::Type::int8     },
          { "i16",  ShaderUniform::Type::int16    },
          { "i32",  ShaderUniform::Type::int32    },
          { "mat4", ShaderUniform::Type::mat4     },
          { "samp", ShaderUniform::Type::sampler  },
    };
    const auto record = nameToType.find(name);
    ASSERT(record != nameToType.end(), "Invalid type Uniform name: {}", name);
    return record->second;
}

std::string ShaderUniform::typeToString(Type type) {
    static const std::unordered_map<ShaderUniform::Type, std::string> typeToName{
        {ShaderUniform::Type::float32,    "f32" },
        { ShaderUniform::Type::float32_2, "vec2"},
        { ShaderUniform::Type::float32_3, "vec3"},
        { ShaderUniform::Type::float32_4, "vec4"},
        { ShaderUniform::Type::uint8,     "u8"  },
        { ShaderUniform::Type::uint16,    "u16" },
        { ShaderUniform::Type::uint32,    "u32" },
        { ShaderUniform::Type::int8,      "i8"  },
        { ShaderUniform::Type::int16,     "i16" },
        { ShaderUniform::Type::int32,     "i32" },
        { ShaderUniform::Type::mat4,      "mat4"},
        { ShaderUniform::Type::sampler,   "samp"},
    };
    const auto record = typeToName.find(type);
    ASSERT(record != typeToName.end(), "Invalid Uniform type: {}", type);
    return record->second;
}

u32 ShaderUniform::getTypeSize(Type type) {
    static const std::unordered_map<ShaderUniform::Type, u32> typeToSize{
        {ShaderUniform::Type::float32,    4 },
        { ShaderUniform::Type::float32_2, 8 },
        { ShaderUniform::Type::float32_3, 12},
        { ShaderUniform::Type::float32_4, 16},
        { ShaderUniform::Type::uint8,     1 },
        { ShaderUniform::Type::uint16,    2 },
        { ShaderUniform::Type::uint32,    4 },
        { ShaderUniform::Type::int8,      1 },
        { ShaderUniform::Type::int16,     2 },
        { ShaderUniform::Type::int32,     4 },
        { ShaderUniform::Type::mat4,      64},
        { ShaderUniform::Type::sampler,   0 },
    };
    const auto record = typeToSize.find(type);
    ASSERT(record != typeToSize.end(), "Invalid Uniform type: {}", type);
    return record->second;
}

void Shader::setGlobalUniforms(SelfCallback&& callback) {
    bindGlobals();
    callback(this);
    applyGlobals();
}

void Shader::setInstanceUniforms(u32 instanceId, SelfCallback&& callback) {
    bindInstance(instanceId);
    callback(this);
    applyInstance();
}

void Shader::setLocalUniforms(SelfCallback&& callback) {
    // nothing to do here, just for consistency
    callback(this);
}

void Shader::use() { impl->use(); }

void Shader::bindGlobals() { impl->bindGlobals(); }

void Shader::bindInstance(u32 instanceId) { impl->bindInstance(instanceId); }

void Shader::applyGlobals() { impl->applyGlobals(); }

void Shader::applyInstance() { impl->applyInstance(); }

u32 Shader::acquireInstanceResources() { return impl->acquireInstanceResources(); }

void Shader::releaseInstanceResources(u32 instanceId) {
    impl->releaseInstanceResources(instanceId);
}

void Shader::setUniform(const std::string& uniform, void* value) {
    impl->setUniform(uniforms[uniform], value);
}

void Shader::setUniform(const std::string& uniform, Texture* value) {
    setUniform(uniform, static_cast<void*>(value));
}

}  // namespace sl
