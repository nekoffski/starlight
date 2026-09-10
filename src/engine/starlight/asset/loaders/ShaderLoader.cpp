#include "ShaderLoader.hh"

#include "starlight/core/Json.hh"

namespace sl {

namespace {

class LoadHelper : public NonMovable, public NonCopyable {
   public:
    Result<ShaderDescription> load(
        std::span<const Byte> data, const Json& metadata
    ) && {
        return {};
    }

   private:
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

    return LoadHelper{}.load(*shaderSource, *metadata);
}

}  // namespace sl
