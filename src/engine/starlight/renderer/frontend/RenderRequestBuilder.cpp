#include "RenderRequestBuilder.hh"

#include <algorithm>
#include <span>
#include <unordered_set>

namespace sl {
namespace {

template <typename Handle>
std::vector<Handle> mergeHandles(
    const std::vector<Handle>& left, const std::vector<Handle>& right
) {
    std::vector<Handle> merged;
    merged.reserve(left.size() + right.size());
    std::unordered_set<Handle> seen;
    seen.reserve(left.size() + right.size());
    for (auto handles : {std::span{left}, std::span{right}}) {
        for (auto handle : handles) {
            if (seen.insert(handle).second) {
                merged.push_back(handle);
            }
        }
    }
    return merged;
}

ShaderResourceDependencies mergeDependencies(
    const ShaderResourceDependencies& left,
    const ShaderResourceDependencies& right
) {
    return {
        .textures = mergeHandles(left.textures, right.textures),
        .samplers = mergeHandles(left.samplers, right.samplers),
    };
}

Result<ShaderParameters> prepareParameters(
    const Material& material, const ShaderParameters& draw
) {
    auto parameters = material.parameters();
    parameters.blocks.insert(
        parameters.blocks.end(), draw.blocks.begin(), draw.blocks.end()
    );
    const auto r = validateShaderParameterBlocks(
        material.shaderInterface().parameters, parameters.blocks
    );
    if (not r) {
        return Error::unexpected(r.error());
    }
    for (const auto& parameter : material.shaderInterface().parameters) {
        const auto block = std::ranges::find(
            parameters.blocks, parameter.name, &ShaderParameterBlock::name
        );
        if (block == parameters.blocks.end()) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument,
                "Missing shader parameter block: {}", parameter.name
            );
        }
    }
    parameters.resources =
        mergeDependencies(material.parameters().resources, draw.resources);
    return parameters;
}

}  // namespace

Result<RenderRequest> RenderRequestBuilder::build() const {
    auto request = m_request;
    for (const auto& draw : m_draws) {
        const auto& material = draw.material.get();
        auto parameters = prepareParameters(material, draw.parameters);
        if (not parameters) {
            return Error::unexpected(parameters.error());
        }
        request.resources =
            mergeDependencies(request.resources, parameters->resources);
        request.scene.items.push_back({
            .primitives = draw.primitives,
            .shader = material.shaderInterface().handle,
            .parameters = std::move(parameters->blocks),
        });
    }
    return request;
}

}  // namespace sl
