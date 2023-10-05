#include "ShaderConfig.h"

#include <numeric>
#include <concepts>

#include <fmt/core.h>

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
      "Stage: [type={}, file={}]", Shader::stageToString(stage), filename
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

}  // namespace sl
