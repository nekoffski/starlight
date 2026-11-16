#include "ShaderLoader.hh"

#include <kcore/Pipeline.hh>

#include "ShaderReflection.hh"

namespace sl {
namespace {

Result<ShaderReflection> loadReflection(
    const kc::FileSystem& filesystem, const kc::Path& path
) {
    if (not filesystem.isFile(path)) {
        return Error::unexpected(
            kc::ErrorCode::fileNotFound, "Shader metadata file not found: {}",
            path.str()
        );
    }
    const auto readMetadata = [&] { return filesystem.readFile(path); };
    return kc::startPipeline(readMetadata)
        .then(kc::parseJson)
        .then(ShaderReflection::parse)
        .result();
}

void logVertexInputs(const ShaderReflection& reflection) {
    if (reflection.vertexInputs) {
        for (const auto& input : *reflection.vertexInputs) {
            log::debug(
                "Shader vertex input {}: {}", input.location,
                valueTypeToString(input.type)
            );
        }
    }
}

}  // namespace

Result<ShaderDescription> ShaderLoader::loadShaderDescription(
    const kc::Path& path
) {
    const kc::FileSystem filesystem;
    if (not filesystem.isFile(path)) {
        return Error::unexpected(
            kc::ErrorCode::fileNotFound, "Shader file not found: {}", path.str()
        );
    }

    const auto readByteCode = [&] { return filesystem.readBinary(path); };
    const auto attachReflection = [&](Bytes byteCode) {
        const auto readReflection = [&] {
            return loadReflection(filesystem, path + ".json");
        };
        const auto assembleDescription = [&](ShaderReflection reflection) {
            return ShaderDescription{
                .stages = std::move(reflection.stages),
                .byteCode = std::move(byteCode),
                .vertexInputs = std::move(reflection.vertexInputs),
                .uniforms = std::move(reflection.uniforms),
                .resourceTables = std::move(reflection.resourceTables),
            };
        };
        return kc::startPipeline(readReflection)
            .inspect(logVertexInputs)
            .transform(assembleDescription)
            .result();
    };
    return kc::startPipeline(readByteCode).then(attachReflection).result();
}

}  // namespace sl
