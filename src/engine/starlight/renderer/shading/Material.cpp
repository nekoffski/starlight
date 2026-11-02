#include "Material.hh"

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
                ErrorCode::invalidArgument,
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
            ErrorCode::invalidArgument, "Invalid material shader handle"
        );
    }
    if (auto r = validateLayout(shaderInterface.parameters); not r) {
        return Error::unexpected(r.error());
    }
    const auto r = validateShaderParameterBlocks(
        shaderInterface.parameters, parameters.blocks
    );
    if (not r) {
        return Error::unexpected(r.error());
    }
    return Material{std::move(shaderInterface), std::move(parameters)};
}

}  // namespace sl
