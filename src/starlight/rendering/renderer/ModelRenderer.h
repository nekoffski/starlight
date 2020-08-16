#pragma once

#include <memory>
#include <unordered_map>

#include "Renderer.h"
#include "lowlevel/LowLevelRenderer.h"
#include "starlight/platform/shader/Shader.h"
#include "starlight/rendering/data/ModelData.h"

// TODO: create FWD
namespace sl::rendering::renderer {

using ShaderPtr = std::shared_ptr<platform::shader::Shader>;
using ModelRenderDataPtr = std::shared_ptr<data::ModelData>;
using ShaderToModelRenderData = std::unordered_map<ShaderPtr, std::vector<data::ModelData>>;

class ModelRenderer : public Renderer {
public:
    using Renderer::Renderer;

    void render(const ShaderToModelRenderData&, const std::shared_ptr<camera::Camera>);

protected:
    void renderModel(const std::shared_ptr<geometry::Model>);
    void renderMesh(const std::shared_ptr<geometry::Mesh>);
};
}