#include "ShaderConfig.h"

#include <numeric>
#include <concepts>

#include <fmt/core.h>

#include "Utils.hpp"

namespace sl {

// it's only for debug purposes but if it'll impact performance consider caching
std::string ShaderAttributeConfig::toString() const {
    return fmt::format(
      "Attribute: [name={}, type={}, size={}]", name,
      ShaderAttribute::typeToString(type), size
    );
}

std::string ShaderUniformConfig::toString() const {
    return fmt::format(
      "Uniform: [name={}, type={}, size={}, location={}, scope={}]", name,
      ShaderUniform::typeToString(type), size, location, shaderScopeToString(scope)
    );
}

std::string ShaderStageConfig::toString() const {
    return fmt::format(
      "Stage: [type={}/{}, file={}]", stage, ShaderStage::typeToString(stage),
      filename
    );
}

template <typename T>
concept RangeConvertibleToString = requires(T t) {
    { t.begin()->toString() };
};

std::string ShaderConfig::toString() const {
    static const auto formatRange =
      [](RangeConvertibleToString auto& range) -> std::string {
        std::string formattedRange;
        for (auto& item : range)
            formattedRange += fmt::format("\n\t\t{},", item.toString());
        return formattedRange;
    };

    return fmt::format(
      "ShaderConfig: [\n\tname={}, use-instances={}, use-local={}, renderpass={},\n"
      "\tstages=[{} \n\t], attributes=[{}\n\t], uniforms=[{}\n\t]"
      "\n]",
      name, useInstances, useLocal, renderpassName, formatRange(stages),
      formatRange(attributes), formatRange(uniforms)
    );
}

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

std::optional<ShaderConfig> ShaderConfig::load(
  const std::string& name, const std::string& shadersPath, const FileSystem& fs
) {
    const auto fullPath = fmt::format("{}/{}.json", shadersPath, name);

    LOG_TRACE("Loading shader config file: {}", fullPath);

    if (not fs.isFile(fullPath)) {
        LOG_ERROR("Could not find file: '{}'", fullPath);
        return {};
    }

    // TODO: extract to free-functions or methods
    auto processStages = [&](const auto& root) -> std::vector<ShaderStageConfig> {
        std::vector<ShaderStageConfig> stages;
        stages.reserve(root.size());

        for (auto& stage : root) {
            const auto file      = getField<std::string>(stage, "file");
            const auto stageName = getField<std::string>(stage, "stage");

            auto source = getShaderSource(shadersPath, file, fs);
            ASSERT(source, "Could not find source file for: {}", file);

            stages.emplace_back(
              ShaderStage::typeFromString(stageName), file, *source
            );
        }
        return stages;
    };

    auto processAttributes =
      [&](const auto& root) -> std::vector<ShaderAttributeConfig> {
        std::vector<ShaderAttributeConfig> attributes;
        attributes.reserve(root.size());

        for (auto& attribute : root) {
            const auto type = ShaderAttribute::typeFromString(
              getField<std::string>(attribute, "type")
            );
            const auto size = ShaderAttribute::getTypeSize(type);
            const auto name = getField<std::string>(attribute, "name");

            attributes.emplace_back(name, type, size);
        }

        return attributes;
    };

    static auto processUniforms =
      [](const auto& root) -> std::vector<ShaderUniformConfig> {
        std::vector<ShaderUniformConfig> uniforms;
        uniforms.reserve(root.size());

        for (auto& uniform : root) {
            const auto type =
              ShaderUniform::typeFromString(getField<std::string>(uniform, "type"));
            const auto size  = ShaderUniform::getTypeSize(type);
            const auto name  = getField<std::string>(uniform, "name");
            const auto scope = getField<std::string>(uniform, "scope");

            uniforms.emplace_back(name, size, 0, type, shaderScopeFromString(scope));
        }

        return uniforms;
    };

    try {
        auto root = kc::json::loadJson(fs.readFile(fullPath));
        ShaderConfig config;

        config.name           = getField<std::string>(root, "name");
        config.renderpassName = getField<std::string>(root, "renderpass");
        config.stages         = processStages(getArray(root, "stages"));
        config.attributes     = processAttributes(getArray(root, "attributes"));
        config.uniforms       = processUniforms(getArray(root, "uniforms"));
        config.useInstances   = getField<bool>(root, "use-instances");
        config.useLocal       = getField<bool>(root, "use-local");

        return config;
    } catch (kc::json::JsonError& e) {
        LOG_ERROR("Could not parse shader '{}' file: {}", name, e.asString());
    }
    return {};
}

}  // namespace sl
