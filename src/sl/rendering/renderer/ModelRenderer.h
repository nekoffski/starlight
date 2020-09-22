#pragma once

#include <memory>
#include <unordered_map>

#include "Renderer.h"
#include "lowlevel/LowLevelRenderer.h"
#include "sl/platform/shader/Shader.h"
#include "sl/rendering/data/ModelData.h"

// TODO: create FWD
namespace sl::rendering::renderer {

class ModelRenderer : public Renderer {
public:
    using Renderer::Renderer;

    void render(std::shared_ptr<platform::shader::Shader> shader, const data::ModelData& modelData,
        const math::Mat4& transform);

protected:
    void renderModel(std::shared_ptr<geometry::Model>);
    void renderMesh(std::shared_ptr<geometry::Mesh>);
};
}