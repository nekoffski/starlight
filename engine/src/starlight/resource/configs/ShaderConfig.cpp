#include "ShaderConfig.h"

#include <numeric>
#include <concepts>

#include <fmt/core.h>

#include "Utils.hpp"

namespace sl {

static std::optional<std::string> getShaderSource(
  const std::string shadersPath, const std::string& name, const FileSystem& fs
) {
    const auto fullPath = fmt::format("{}/{}", shadersPath, name);

    if (not fs.isFile(fullPath)) {
        LOG_WARN("Could not find shader file '{}'", fullPath);
        return {};
    }
    return fs.readFile(fullPath);
}

static std::vector<Shader::Stage> processStages(
  const kc::json::Node& root, const std::string& shadersPath, const FileSystem& fs
) {
    std::vector<Shader::Stage> stages;
    stages.reserve(root.size());

    for (auto& stage : root) {
        const auto file      = getField<std::string>(stage, "file");
        const auto stageName = getField<std::string>(stage, "stage");

        auto source = getShaderSource(shadersPath, file, fs);
        ASSERT(source, "Could not find source file for: {}", file);

        stages.emplace_back(Shader::Stage::typeFromString(stageName), *source);
    }
    return stages;
}
std::vector<Shader::Attribute> processAttributes(const kc::json::Node& root) {
    std::vector<Shader::Attribute> attributes;
    attributes.reserve(root.size());

    for (auto& attribute : root) {
        const auto type = Shader::Attribute::typeFromString(
          getField<std::string>(attribute, "type")
        );
        const auto size = Shader::Attribute::getTypeSize(type);
        const auto name = getField<std::string>(attribute, "name");

        attributes.emplace_back(name, type, size);
    }

    return attributes;
};

std::vector<Shader::Uniform::Properties> processUniforms(const kc::json::Node& root
) {
    std::vector<Shader::Uniform::Properties> uniforms;
    uniforms.reserve(root.size());

    static auto getSize =
      [](const kc::json::Node& uniform, Shader::Uniform::Type type) -> u64 {
        if (type == Shader::Uniform::Type::custom) {
            auto size         = getField<unsigned int>(uniform, "size");
            auto elementCount = getField<unsigned int>(uniform, "elements");

            return size * elementCount;
        } else {
            return Shader::Uniform::getTypeSize(type);
        }
    };

    for (auto& uniform : root) {
        const auto type =
          Shader::Uniform::typeFromString(getField<std::string>(uniform, "type"));

        const auto size  = getSize(uniform, type);
        const auto name  = getField<std::string>(uniform, "name");
        const auto scope = getField<std::string>(uniform, "scope");

        uniforms.emplace_back(name, size, 0, type, Shader::scopeFromString(scope));
    }

    return uniforms;
};

std::optional<ShaderConfig> ShaderConfig::load(
  const std::string& name, Texture* defaultTexture, const std::string& shadersPath,
  const FileSystem& fs
) {
    const auto fullPath = fmt::format("{}/{}.json", shadersPath, name);

    LOG_TRACE("Loading shader config file: {}", fullPath);

    if (not fs.isFile(fullPath)) {
        LOG_ERROR("Could not find file: '{}'", fullPath);
        return {};
    }

    try {
        auto root = kc::json::loadJson(fs.readFile(fullPath));
        // clang-format off
        return ShaderConfig{
            Shader::Properties{
                .name           = getField<std::string>(root, "name"),
                .useInstances   = getField<bool>(root, "use-instances"),
                .useLocals      = getField<bool>(root, "use-local"),
                .attributes     = processAttributes(getArray(root, "attributes")),
                .stages = processStages(getArray(root, "stages"), shadersPath, fs),
                .uniformProperties = processUniforms(getArray(root, "uniforms")),
                .defaultTexture    = defaultTexture,
                .cullMode = cullModeFromString(getFieldOr<std::string>(root, "cullMode", "back"))
            }
        };
        // clang-format on
    } catch (kc::json::JsonError& e) {
        LOG_ERROR("Could not parse shader '{}' file: {}", name, e.asString());
    }
    return {};
}

}  // namespace sl
