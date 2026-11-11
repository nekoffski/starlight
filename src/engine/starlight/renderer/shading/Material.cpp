#include "Material.hh"

#include <kcore/Pipeline.hh>
#include <string_view>
#include <unordered_set>

namespace sl {
namespace {

using ParameterNames = std::unordered_set<std::string_view>;

Result<void> validateLayout(std::span<const ShaderParameterLayout> layout) {
    ParameterNames names;
    names.reserve(layout.size());
    for (const auto& parameter : layout) {
        const bool validLayout = not parameter.name.empty() &&
                                 parameter.size > 0 &&
                                 names.insert(parameter.name).second;
        if (not validLayout) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument,
                "Invalid shader parameter layout: {}", parameter.name
            );
        }
    }
    return {};
}

}  // namespace

Material::Material(ShaderInterface shaderInterface, ShaderParameters parameters)
    : m_shaderInterface(std::move(shaderInterface)),
      m_parameters(std::move(parameters)) {}

Result<Material> Material::create(
    ShaderInterface shaderInterface, ShaderParameters parameters
) {
    if (not shaderInterface.handle.valid()) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Invalid material shader handle"
        );
    }
    const auto validateInterface = [&] {
        return validateLayout(shaderInterface.parameters);
    };
    const auto validateParameters = [&] {
        return validateShaderParameterBlocks(
            shaderInterface.parameters, parameters.blocks
        );
    };
    const auto createMaterial = [&] {
        return Material{std::move(shaderInterface), std::move(parameters)};
    };
    return kc::startPipeline(validateInterface)
        .then(validateParameters)
        .transform(createMaterial)
        .result();
}

}  // namespace sl
