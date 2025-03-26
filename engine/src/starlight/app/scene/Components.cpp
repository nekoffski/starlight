#include "Components.hh"

#include "Entity.hh"

namespace sl {

void TransformComponent::onInit() {
    if (auto& entity = getEntity(); entity.has<ModelComponent>()) {
        auto& parent = data();
        entity.get<ModelComponent>()->data().getTransform().setParent(parent);
    }
}

}  // namespace sl
