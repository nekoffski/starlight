#include "ShaderLoader.hh"

#include "starlight/core/Json.hh"
#include "starlight/core/Log.hh"

namespace sl {

namespace {

class LoadHelper : public NonMovable, public NonCopyable {
   public:
    Result<ShaderDescription> load(
        std::vector<Byte> data, const Json& metadata
    ) &&;

   private:
    Result<ShaderModuleDescription> loadModule(const Json& metadata);

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
        if (auto module = loadModule(entrypoint); module) {
            m_shaderDescription.stages[module->stage] = *module;
        } else {
            return Error::unexpected(module.error());
        }
    }

    m_shaderDescription.byteCode = std::move(data);
    return std::move(m_shaderDescription);
}

Result<ShaderModuleDescription> LoadHelper::loadModule(const Json& metadata) {
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

    auto entryPoint = metadata.find("name");

    if (entryPoint == metadata.end() || not entryPoint->is_string()) {
        return Error::unexpected(
            ErrorCode::shaderParsingError, "Invalid shader module entry point"
        );
    }

    shaderModule.entryPoint = entryPoint.value();

    return shaderModule;
}

}  // namespace

}  // namespace sl
