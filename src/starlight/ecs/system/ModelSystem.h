// #pragma once

// #include "System.h"

// #include "starlight/ecs/component/ModelComponent.h"
// #include "starlight/math/Utils.hpp"
// // TODO: move FWD to another file
// #include "starlight/core/log/Logger.h"
// #include "starlight/rendering/renderer/ModelRenderer.h"

// namespace sl::ecs::system {

// class ModelSystem : public System {
// public:
//     void clean() {
//         m_models.clear();
//     }

//     void processComponent(component::Component& component, std::shared_ptr<entity::Entity>& entity) {
//         // processComponentImpl()
//     }

//     rendering::renderer::ShaderToModelRenderData& getModels() {
//         return m_models;
//     }

//     void processComponentImpl(component::ModelComponent& component, const std::shared_ptr<entity::Entity>& entity) {
//         // auto translation = math::translate(entity->getPosition());
//         // auto rotation = math::createRotationMatrix(math::toRadians(math::Vec));
//         // auto transform = std::move(translation) * std::move(rotation);

//         auto renderData = *component.getRenderData();
//         for (auto& modelMatrix : renderData.modelMatrices)
//             modelMatrix = modelMatrix; // keep in mind multiplication order
//         m_models[component.getShader()].push_back(std::move(renderData));
//     }

// private:
//     rendering::renderer::ShaderToModelRenderData m_models;
// };
// }
