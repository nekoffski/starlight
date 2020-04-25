#pragma once

#include <memory>
#include <unordered_map>

#include <starlight/framework/graphics/LowLevelRenderer.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/rendering/data/InstancedModelRenderData.h>
#include <starlight/rendering/renderer/ModelRenderer.h>

// TODO: create FWD
namespace starl::rendering::renderer {

using ShaderPtr = std::shared_ptr<shader::Shader>;
using InstancedModelRenderDataPtr = std::shared_ptr<data::InstancedModelRenderData>;
using ShaderToInstancedModelRenderData = std::unordered_map<ShaderPtr, std::vector<InstancedModelRenderDataPtr>>;

class InstancedModelRenderer : public ModelRenderer {
public:
    using ModelRenderer::ModelRenderer;

    void render(const ShaderToInstancedModelRenderData&);
};
}