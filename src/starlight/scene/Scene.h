#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <starlight/rendering/entity/ModelRenderEntity.h>

namespace starl {
namespace platform::shader {
    class Shader;
}

namespace rendering::entity {
    class ModelRenderEntity;
}
}

namespace starl::scene {

using ShaderPtr = std::shared_ptr<platform::shader::Shader>;
using ModelRenderEntityPtr = std::shared_ptr<rendering::entity::ModelRenderEntity>;
using ShaderToModelRenderEntities = std::unordered_map<ShaderPtr, std::vector<ModelRenderEntityPtr>>;

class Scene {
public:
    void pushModel(const ShaderPtr& shader, std::shared_ptr<geometry::Model>& model) {
        m_entitiesMap[shader].push_back(std::make_shared<rendering::entity::ModelRenderEntity>(model));
    }

private:
    ShaderToModelRenderEntities m_entitiesMap;
};
}
