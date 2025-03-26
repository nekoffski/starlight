#pragma once

#include "Component.hh"

#include "starlight/renderer/light/DirectionalLight.hh"
#include "starlight/renderer/light/PointLight.hh"
#include "starlight/app/model/Model.hh"
#include "starlight/physx/Transform.hh"

namespace sl {

struct DirectionalLightComponent : Component<DirectionalLight> {
    using Component::Component;
};

struct PointLightComponent : Component<PointLight> {
    using Component::Component;
};

struct ModelComponent : Component<SharedPtr<Model>> {
    using Component::Component;
};

struct TransformComponent : Component<Transform> {
    using Component::Component;

    void onInit() override;
};

}  // namespace sl
