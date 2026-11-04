#include "ShaderLoader.hh"

#include <kcore/Functional.hh>

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
    return filesystem.readFile(path)
        .and_then(kc::parseJson)
        .and_then(ShaderReflection::parse);
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

    return filesystem.readBinary(path).and_then([&](Bytes byteCode) {
        return loadReflection(filesystem, path + ".json")
            .transform([bc = std::move(byteCode)](auto reflection) mutable {
                if (reflection.vertexLayout) {
                    log::debug(
                        "Shader layout: {}",
                        vertexLayoutToString(*reflection.vertexLayout)
                    );
                }
                return ShaderDescription{
                    .stages = std::move(reflection.stages),
                    .byteCode = std::move(bc),
                    .vertexLayout = std::move(reflection.vertexLayout),
                    .uniforms = std::move(reflection.uniforms),
                    .resourceTables = std::move(reflection.resourceTables),
                };
            });
    });
}

}  // namespace sl
