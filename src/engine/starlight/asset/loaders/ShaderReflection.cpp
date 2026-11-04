#include "ShaderReflection.hh"

#include <algorithm>
#include <limits>
#include <unordered_set>
#include <variant>

#include "starlight/renderer/core/Texture.hh"

namespace sl {
namespace {

using ParameterBlock = std::variant<ShaderUniform, ShaderResourceTable>;

struct ParameterBlocks {
    std::vector<ShaderUniform> uniforms;
    std::vector<ShaderResourceTable> resourceTables;
};

template <typename T>
Result<T> invalidReflection(const Str& message) {
    return Error::unexpected(ErrorCode::shaderParsingError, "{}", message);
}

const kc::Json::array_t& arrayField(const kc::Json& object, const char* name) {
    return object.at(name).get_ref<const kc::Json::array_t&>();
}

Result<u32> parseU32(const kc::Json& value) {
    const bool nonnegativeInteger = value.is_number_integer() && value >= 0;
    if (not nonnegativeInteger || value > std::numeric_limits<u32>::max()) {
        return invalidReflection<u32>(
            "Expected a non-negative 32-bit layout value"
        );
    }
    return value.get<u32>();
}

Result<ShaderModuleDescription> parseEntryPoint(const kc::Json& entryPoint) {
    const auto stageName = entryPoint.at("stage").get<Str>();
    auto stage = shaderStageFromString(stageName);
    if (not stage) {
        return invalidReflection<ShaderModuleDescription>(
            "Unsupported shader module stage: " + stageName
        );
    }

    auto name = entryPoint.at("name").get<Str>();
    if (name.empty()) {
        return invalidReflection<ShaderModuleDescription>(
            "Empty shader module entry point"
        );
    }
    return ShaderModuleDescription{*stage, std::move(name)};
}

Result<ValueType> parseVertexType(const kc::Json& type) {
    const auto kind = type.at("kind").get<Str>();
    if (kind != "vector") {
        return invalidReflection<ValueType>(
            "Unsupported vertex input type: " + kind
        );
    }
    if (type.at("elementType").at("scalarType").get<Str>() != "float32") {
        return invalidReflection<ValueType>(
            "Vertex inputs require float32 vectors"
        );
    }

    const auto count = parseU32(type.at("elementCount"));
    if (not count) {
        return Error::unexpected(count.error());
    }
    switch (*count) {
        case 3:
            return ValueType::f32_3x1;
        case 4:
            return ValueType::f32_4x1;
        default:
            return invalidReflection<ValueType>(
                "Vertex inputs require three or four elements"
            );
    }
}

Result<VertexLayout> parseVertexInputs(const kc::Json& entryPoint) {
    VertexLayout layout;
    for (const auto& parameter : arrayField(entryPoint, "parameters")) {
        if (not parameter.is_object()) {
            return invalidReflection<VertexLayout>(
                "Expected a vertex parameter object"
            );
        }
        if (not parameter.contains("binding")) {
            continue;
        }
        if (not parameter.at("binding").is_object()) {
            return invalidReflection<VertexLayout>(
                "Expected a vertex binding object"
            );
        }

        auto type = parseVertexType(parameter.at("type"));
        if (not type) {
            return Error::unexpected(type.error());
        }
        layout.push_back(*type);
    }
    return layout;
}

Result<u32> parseUniformSize(const kc::Json& element) {
    u32 size = 0;
    for (const auto& entry : arrayField(element, "sizes")) {
        if (entry.at("kind").get<Str>() != "uniform") {
            continue;
        }
        const auto value = parseU32(entry.at("value"));
        if (not value) {
            return Error::unexpected(value.error());
        }
        size = *value;
    }
    return size;
}

Result<ShaderResourceKind> parseResourceKind(const kc::Json& resource) {
    const auto kind = resource.at("kind").get<Str>();
    if (kind == "samplerState") {
        return ShaderResourceKind::sampler;
    }
    const bool sampledTexture =
        kind == "resource" &&
        resource.at("baseShape").get<Str>() == "texture2D" &&
        resource.value("access", Str("read")) == "read";
    if (sampledTexture) {
        return ShaderResourceKind::texture;
    }
    return invalidReflection<ShaderResourceKind>(
        "Unsupported resource table element"
    );
}

Result<ShaderResourceArray> parseResourceArray(const kc::Json& field) {
    const auto& array = field.at("type");
    if (array.at("kind").get<Str>() != "array") {
        return invalidReflection<ShaderResourceArray>(
            "Resource tables must contain only arrays"
        );
    }

    const auto kind = parseResourceKind(array.at("elementType"));
    if (not kind) {
        return Error::unexpected(kind.error());
    }
    const auto count = parseU32(array.at("elementCount"));
    if (not count) {
        return Error::unexpected(count.error());
    }
    if (*count == 0 || *count > resourceTableCapacity) {
        return invalidReflection<ShaderResourceArray>(
            "Unsupported resource table capacity"
        );
    }

    const auto& location = field.at("binding");
    const auto offset = parseU32(location.value("offset", kc::Json(0u)));
    if (not offset) {
        return Error::unexpected(offset.error());
    }
    const auto stride = parseU32(array.value("uniformStride", kc::Json(0u)));
    if (not stride) {
        return Error::unexpected(stride.error());
    }
    const auto binding = parseU32(location.value("index", kc::Json(0u)));
    if (not binding) {
        return Error::unexpected(binding.error());
    }
    return ShaderResourceArray{*kind, *count, *offset, *stride, *binding};
}

Result<std::vector<ShaderResourceArray>> parseResourceArrays(
    const kc::Json::array_t& fields
) {
    std::vector<ShaderResourceArray> arrays;
    arrays.reserve(fields.size());
    for (const auto& field : fields) {
        auto array = parseResourceArray(field);
        if (not array) {
            return Error::unexpected(array.error());
        }
        arrays.push_back(*array);
    }
    return arrays;
}

Result<ParameterBlock> parseParameterBlock(const kc::Json& parameter) {
    auto name = parameter.at("name").get<Str>();
    const auto binding = parseU32(parameter.at("binding").at("index"));
    if (not binding) {
        return Error::unexpected(binding.error());
    }
    const auto& type = parameter.at("type");
    const auto kind = type.at("kind").get<Str>();
    if (kind != "constantBuffer" && kind != "parameterBlock") {
        return invalidReflection<ParameterBlock>(
            "Only constant buffers and resource parameter blocks are supported"
        );
    }
    const auto& element = type.at("elementVarLayout").at("type");
    const auto size = parseUniformSize(element);
    if (not size) {
        return Error::unexpected(size.error());
    }
    const auto& fields = arrayField(element, "fields");
    const bool resourceTable =
        std::ranges::any_of(fields, [](const kc::Json& field) {
            return field.at("type").at("kind").get<Str>() == "array";
        });
    if (not resourceTable) {
        if (*size == 0) {
            return invalidReflection<ParameterBlock>("Empty uniform block");
        }
        return ShaderUniform{*binding, *size, std::move(name)};
    }

    auto arrays = parseResourceArrays(fields);
    if (not arrays) {
        return Error::unexpected(arrays.error());
    }
    return ShaderResourceTable{
        std::move(name), *binding, *size, std::move(*arrays)
    };
}

Result<ParameterBlocks> parseParameterBlocks(const kc::Json& globalScope) {
    if (not globalScope.is_object()) {
        return invalidReflection<ParameterBlocks>(
            "Expected a globalScope object"
        );
    }
    ParameterBlocks blocks;
    if (not globalScope.contains("parameters")) {
        return blocks;
    }

    std::unordered_set<Str> names;
    for (const auto& parameter : arrayField(globalScope, "parameters")) {
        auto block = parseParameterBlock(parameter);
        if (not block) {
            return Error::unexpected(block.error());
        }
        const auto& name = std::visit(
            [](const auto& value) -> const Str& { return value.name; }, *block
        );
        if (name.empty() || not names.insert(name).second) {
            return invalidReflection<ParameterBlocks>(
                "Empty or duplicate shader parameter name"
            );
        }
        if (auto* uniform = std::get_if<ShaderUniform>(&*block)) {
            blocks.uniforms.push_back(std::move(*uniform));
        } else {
            blocks.resourceTables.push_back(
                std::get<ShaderResourceTable>(std::move(*block))
            );
        }
    }
    return blocks;
}

}  // namespace

Result<ShaderReflection> ShaderReflection::parse(const kc::Json& metadata) {
    try {
        ShaderReflection reflection;
        for (const auto& entryPoint : arrayField(metadata, "entryPoints")) {
            auto module = parseEntryPoint(entryPoint);
            if (not module) {
                return Error::unexpected(module.error());
            }
            if (module->stage == ShaderStage::vertex) {
                auto layout = parseVertexInputs(entryPoint);
                if (not layout) {
                    return Error::unexpected(layout.error());
                }
                reflection.vertexLayout = std::move(*layout);
            }
            reflection.stages[module->stage] = std::move(*module);
        }

        if (const auto globalScope = metadata.find("globalScope");
            globalScope != metadata.end()) {
            auto blocks = parseParameterBlocks(*globalScope);
            if (not blocks) {
                return Error::unexpected(blocks.error());
            }
            reflection.uniforms = std::move(blocks->uniforms);
            reflection.resourceTables = std::move(blocks->resourceTables);
        }
        return reflection;
    } catch (const kc::Json::exception& error) {
        return Error::unexpected(
            ErrorCode::shaderParsingError, "Invalid shader reflection: {}",
            error.what()
        );
    }
}

}  // namespace sl
