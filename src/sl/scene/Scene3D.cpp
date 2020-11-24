#include "Scene3D.h"

#include <memory>
#include <unordered_map>
#include <vector>

#include "sl/ecs/Entity.h"

#include "sl/scene/components/DirectionalLightComponentWrapper.h"
#include "sl/scene/components/MaterialComponentWrapper.h"
#include "sl/scene/components/ParticleEffectComponentWrapper.h"
#include "sl/scene/components/PointLightComponentWrapper.h"
#include "sl/scene/components/TransformComponentWrapper.h"

namespace sl::scene {

std::shared_ptr<Scene3D> Scene3D::create() {
    auto scene = std::make_shared<Scene3D>();
    scene->setComponentWrapperFactory<components::TransformComponent,
        components::TransformComponentWrapperFactory>();
    scene->setComponentWrapperFactory<components::MaterialComponent,
        components::MaterialComponentWrapperFactory>();
    scene->setComponentWrapperFactory<components::DirectionalLightComponent,
        components::DirectionalLightComponentWrapperFactory>();
    scene->setComponentWrapperFactory<components::PointLightComponent,
        components::PointLightComponentWrapperFactory>();
    scene->setComponentWrapperFactory<components::ParticleEffectComponent,
        components::ParticleEffectComponentWrapperFactory>();
    return scene;
}

void Scene3D::setSkybox(std::shared_ptr<Skybox> skybox) {
    m_skybox = std::move(skybox);
}

}
