#include "ShaderLoader.hh"

#include <algorithm>
#include <unordered_set>

#include "starlight/core/Json.hh"
#include "starlight/core/Log.hh"
#include "starlight/renderer/core/Texture.hh"

namespace sl {

namespace {

class LoadHelper : public NonMovable, public NonCopyable {
   public:
    Result<ShaderDescription> load(
        std::vector<Byte> data, const Json& metadata
    ) &&;

   private:
    Result<void> loadModule(const Json& metadata);
    Result<void> parseVertexInput(const Json& metadata);
    Result<void> parseUniforms(const Json& metadata);

    ShaderDescription m_shaderDescription;
};

}  // namespace

Result<ShaderDescription> ShaderLoader::loadShaderDescription(
    const Path& path
) {
    if (not path.isFile()) {
        return Error::unexpected(
            ErrorCode::fileNotFound, "Shader file not found: {}", path.str()
        );
    }

    const auto shaderSource = File{path}.readBinary();

    if (not shaderSource) {
        return Error::unexpected(shaderSource.error());
    }

    const auto metadataPath = path + ".json";

    if (not metadataPath.isFile()) {
        return Error::unexpected(
            ErrorCode::fileNotFound, "Shader metadata file not found: {}",
            metadataPath.str()
        );
    }

    const auto rawMetadata = File{metadataPath}.read();

    if (not rawMetadata) {
        return Error::unexpected(rawMetadata.error());
    }

    auto metadata = parseJson(*rawMetadata);

    if (not metadata) {
        return Error::unexpected(metadata.error());
    }

    return LoadHelper{}.load(std::move(*shaderSource), *metadata);
}

namespace {

Result<ShaderDescription> LoadHelper::load(
    std::vector<Byte> data, const Json& metadata
) && {
    auto entrypoints = metadata.find("entryPoints");

    if (entrypoints == metadata.end() || not entrypoints->is_array()) {
        return Error::unexpected(
            ErrorCode::shaderParsingError,
            "Shader metadata is missing required field: 'entryPoints'"
        );
    }

    for (const auto& entrypoint : entrypoints.value()) {
        if (auto res = loadModule(entrypoint); not res) {
            return Error::unexpected(res.error());
        }
    }

    if (auto globalScope = metadata.find("globalScope");
        globalScope != metadata.end() && globalScope->is_object()) {
        if (auto parameters = globalScope->find("parameters");
            parameters != globalScope->end() && parameters->is_array()) {
            if (auto res = parseUniforms(*parameters); not res) {
                return Error::unexpected(res.error());
            }
        }
    }

    m_shaderDescription.byteCode = std::move(data);
    return std::move(m_shaderDescription);
}

Result<void> LoadHelper::loadModule(const Json& metadata) {
    ShaderModuleDescription shaderModule;

    auto stageStr = metadata.find("stage");

    if (stageStr == metadata.end() || not stageStr->is_string()) {
        return Error::unexpected(
            ErrorCode::shaderParsingError, "Invalid shader module stage"
        );
    }

    auto stage = shaderStageFromString(stageStr.value());

    if (not stage) {
        return Error::unexpected(
            ErrorCode::shaderParsingError,
            "Unsupported shader module stage: {}", stage.error()
        );
    }

    shaderModule.stage = *stage;

    if (shaderModule.stage == ShaderStage::vertex) {
        auto parameters = metadata.find("parameters");

        if (parameters == metadata.end() || not parameters->is_array()) {
            return Error::unexpected(
                ErrorCode::shaderParsingError,
                "Shader module is missing required field: 'parameters'"
            );
        }

        if (auto res = parseVertexInput(*parameters); not res) {
            return Error::unexpected(res.error());
        }
    }

    auto entryPoint = metadata.find("name");

    if (entryPoint == metadata.end() || not entryPoint->is_string()) {
        return Error::unexpected(
            ErrorCode::shaderParsingError, "Invalid shader module entry point"
        );
    }

    shaderModule.entryPoint = entryPoint.value();
    m_shaderDescription.stages[shaderModule.stage] = shaderModule;
    return {};
}

namespace {

Result<ValueType> parseValueType(const Json& type) {
    auto kind = type.find("kind");

    if (kind == type.end() || not kind->is_string()) {
        return Error::unexpected(
            ErrorCode::shaderParsingError, "Invalid value type kind"
        );
    }

    if (kind.value() == "vector") {
        auto elementCountObj = type.find("elementCount");

        if (elementCountObj == type.end() || not elementCountObj->is_number()) {
            return Error::unexpected(
                ErrorCode::shaderParsingError, "Invalid vector element count"
            );
        }

        auto elementType = type.find("elementType");

        if (elementType == type.end() || not elementType->is_object()) {
            return Error::unexpected(
                ErrorCode::shaderParsingError, "Invalid vector element type"
            );
        }

        auto scalarType = elementType->find("scalarType");

        if (scalarType == elementType->end() || not scalarType->is_string()) {
            return Error::unexpected(
                ErrorCode::shaderParsingError,
                "Invalid vector element scalar type"
            );
        }

        if (scalarType.value() == "float32") {
            switch (elementCountObj->get<int>()) {
                case 3:
                    return ValueType::f32_3x1;
                case 4:
                    return ValueType::f32_4x1;
                default:
                    return Error::unexpected(
                        ErrorCode::shaderParsingError,
                        "Invalid vector element count for float32 type"
                    );
            }
        }
    }

    return Error::unexpected(
        ErrorCode::shaderParsingError, "Unhandled value type kind: {}",
        kind.value().dump()
    );
}

}  // namespace

Result<void> LoadHelper::parseVertexInput(const Json& parameters) {
    VertexLayout layout;

    for (const auto& parameter : parameters) {
        if (auto binding = parameter.find("binding");
            binding == parameter.end()) {
            continue;
        }

        auto type = parameter.find("type");

        if (type == parameter.end() || not type->is_object()) {
            return Error::unexpected(
                ErrorCode::shaderParsingError, "Invalid vertex input type"
            );
        }

        if (auto internalType = parseValueType(*type); not internalType) {
            return Error::unexpected(internalType.error());
        } else {
            layout.push_back(*internalType);
        }
    }

    log::debug("Shader layout: {}", vertexLayoutToString(layout));
    m_shaderDescription.vertexLayout = std::move(layout);
    return {};
}

Result<void> LoadHelper::parseUniforms(const Json& metadata) {
    try {
        const auto number = [](const Json& value) -> u32 {
            if (not value.is_number_unsigned() ||
                value.get<u64>() > std::numeric_limits<u32>::max()) {
                throw std::runtime_error(
                    "Expected a non-negative 32-bit layout value"
                );
            }
            return value.get<u32>();
        };
        std::unordered_set<Str> names;
        for (const auto& parameter : metadata) {
            const auto name = parameter.at("name").get<Str>();
            if (name.empty() || not names.insert(name).second) {
                throw std::runtime_error(
                    "Empty or duplicate shader parameter name"
                );
            }
            const auto binding = number(parameter.at("binding").at("index"));
            const auto& type = parameter.at("type");
            const auto kind = type.at("kind").get<Str>();
            if (kind != "constantBuffer" && kind != "parameterBlock") {
                throw std::runtime_error(
                    "Only constant buffers and resource parameter blocks are "
                    "supported"
                );
            }
            const auto& element = type.at("elementVarLayout").at("type");
            u32 size = 0;
            for (const auto& entry : element.at("sizes")) {
                if (entry.at("kind") == "uniform") {
                    size = number(entry.at("value"));
                }
            }
            const auto& fields = element.at("fields");
            if (not fields.is_array()) {
                throw std::runtime_error("Invalid block fields");
            }
            const bool resourceTable =
                std::ranges::any_of(fields, [](const auto& field) {
                    return field.at("type").at("kind") == "array";
                });
            if (not resourceTable) {
                if (size == 0) {
                    throw std::runtime_error("Empty uniform block");
                }
                m_shaderDescription.uniforms.push_back({binding, size, name});
                continue;
            }
            ShaderResourceTable table{name, binding, size, {}};
            for (const auto& field : fields) {
                const auto& array = field.at("type");
                if (array.at("kind") != "array") {
                    throw std::runtime_error(
                        "Resource tables must contain only arrays"
                    );
                }
                const auto& resource = array.at("elementType");
                ShaderResourceKind resourceKind;
                if (resource.at("kind") == "resource" &&
                    resource.at("baseShape") == "texture2D" &&
                    resource.value("access", Str("read")) == "read") {
                    resourceKind = ShaderResourceKind::texture;
                } else if (resource.at("kind") == "samplerState") {
                    resourceKind = ShaderResourceKind::sampler;
                } else {
                    throw std::runtime_error(
                        "Unsupported resource table element"
                    );
                }
                const auto count = number(array.at("elementCount"));
                if (count == 0 || count > resourceTableCapacity) {
                    throw std::runtime_error(
                        "Unsupported resource table capacity"
                    );
                }
                const auto& location = field.at("binding");
                table.arrays.push_back(
                    {resourceKind, count,
                     number(location.value("offset", Json(0u))),
                     number(array.value("uniformStride", Json(0u))),
                     number(location.value("index", Json(0u)))}
                );
            }
            m_shaderDescription.resourceTables.push_back(std::move(table));
        }
    } catch (const std::exception& error) {
        return Error::unexpected(
            ErrorCode::shaderParsingError,
            "Invalid shader parameter layout: {}", error.what()
        );
    }
    return {};
}

}  // namespace

}  // namespace sl
