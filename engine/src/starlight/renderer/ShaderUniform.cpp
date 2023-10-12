#include "ShaderUniform.h"

#include <unordered_map>
#include <string_view>

#include "starlight/core/utils/Log.h"

namespace sl {

namespace {

static const std::unordered_map<std::string_view, ShaderUniform::Type> nameToType{
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

}  // namespace

bool ShaderUniform::isSampler() const { return type == Type::sampler; }

ShaderUniform::Type ShaderUniform::typeFromString(const std::string& name) {
    const auto record = nameToType.find(name);
    ASSERT(record != nameToType.end(), "Invalid type Uniform name: {}", name);
    return record->second;
}

std::string ShaderUniform::typeToString(Type type) {
    const auto record = typeToName.find(type);
    ASSERT(record != typeToName.end(), "Invalid Uniform type: {}", type);
    return record->second;
}

u32 ShaderUniform::getTypeSize(Type type) {
    const auto record = typeToSize.find(type);
    ASSERT(record != typeToSize.end(), "Invalid Uniform type: {}", type);
    return record->second;
}

}  // namespace sl
