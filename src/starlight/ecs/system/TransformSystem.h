// #pragma once

// #include "System.h"

// #include "starlight/ecs/component/TransformComponent.h"

// namespace sl::ecs::system {

// class TransformSystem : public System {
// public:
//     void processComponent(std::shared_ptr<component::Component>& component, std::shared_ptr<entity::Entity>& entity) override {
//         processComponentImpl(std::dynamic_pointer_cast<component::TransformComponent>(component), entity);
//     }

// private:
//     void processComponentImpl(std::shared_ptr<component::TransformComponent> component, std::shared_ptr<entity::Entity>& entity) {
//         entity->setPosition(component->getTranslation());
//         entity->setOrientation(component->getRotation());
//     }
// };
// }
