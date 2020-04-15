#pragma once

#include <memory>
#include <unordered_map>

#include <starlight/framework/graphics/LowLevelRenderer.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/rendering/entity/InstancedModelRenderEntity.h>
#include <starlight/rendering/renderer/ModelRenderer.h>

// TODO: create FWD
namespace starl::rendering::renderer {

using ShaderPtr = std::shared_ptr<shader::Shader>;
using InstancedModelRenderEntityPtr = std::shared_ptr<entity::InstancedModelRenderEntity>;
using ShaderToInstancedModelRenderEntities = std::unordered_map<ShaderPtr, std::vector<InstancedModelRenderEntityPtr>>;

class InstancedModelRenderer : public ModelRenderer {
public:
    using ModelRenderer::ModelRenderer;

    void render(const ShaderToInstancedModelRenderEntities&);
};
}