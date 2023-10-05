#include "ShaderAttribute.h"

#include <unordered_map>
#include <string_view>

#include "starlight/core/utils/Log.h"

namespace sl {

namespace {

static const std::unordered_map<std::string_view, ShaderAttribute::Type> nameToType{
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

}  // namespace

ShaderAttribute::Type ShaderAttribute::typeFromString(const std::string& name) {
    const auto record = nameToType.find(name);
    ASSERT(record != nameToType.end(), "Invalid type attribute name: {}", name);
    return record->second;
}

std::string ShaderAttribute::typeToString(Type type) {
    const auto record = typeToName.find(type);
    ASSERT(record != typeToName.end(), "Invalid attribute type: {}", type);
    return record->second;
}

u32 ShaderAttribute::getTypeSize(Type type) {
    const auto record = typeToSize.find(type);
    ASSERT(record != typeToSize.end(), "Invalid attribute type: {}", type);
    return record->second;
}

}  // namespace sl
