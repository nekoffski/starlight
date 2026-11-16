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

Result<void> validateCompleteParameters(
    std::span<const ShaderParameterLayout> layout,
    std::span<const ShaderParameterBlock> blocks
) {
    const auto r = validateShaderParameterBlocks(layout, blocks);
    if (not r) {
        return r;
    }
    for (const auto& parameter : layout) {
        const auto block = std::ranges::find(
            blocks, parameter.name, &ShaderParameterBlock::name
        );
        if (block == blocks.end()) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument,
                "Missing shader parameter block: {}", parameter.name
            );
        }
    }
    return {};
}

Result<ShaderParameters> prepareParameters(
    const Material& material, const ShaderParameters& draw,
    std::span<const RenderView> views
) {
    ShaderParameters parameters{
        .blocks = combineShaderParameterBlocks(
            material.parameters().blocks, draw.blocks
        )
    };
    const auto& layout = material.shaderInterface().parameters;
    if (views.empty()) {
        const auto r = validateCompleteParameters(layout, parameters.blocks);
        if (not r) {
            return Error::unexpected(r.error());
        }
    }
    for (const auto& view : views) {
        const auto blocks =
            combineShaderParameterBlocks(parameters.blocks, view.parameters);
        const auto r = validateCompleteParameters(layout, blocks);
        if (not r) {
            return Error::unexpected(r.error());
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
        auto parameters =
            prepareParameters(material, draw.parameters, request.views);
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
