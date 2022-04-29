#include "FirstPersonController.h"

#include "components/TransformComponent.h"
#include "components/CameraComponent.h"

namespace sl::scene {

void createFirstPersonController(ecs::Entity& targetEntity) {
    targetEntity.addComponent<components::TransformComponent>();
    targetEntity.addComponent<components::CameraComponent>();
}

}  // namespace sl::scene