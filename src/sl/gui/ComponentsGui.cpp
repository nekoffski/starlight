#include "ComponentsGui.h"

#include "sl/core/Misc.hpp"

#include "components/DirectionalLightComponentGui.h"
#include "components/MaterialComponentGui.h"
#include "components/MeshRendererComponentGui.h"
#include "components/ModelComponentGui.h"
#include "components/ParticleEffectComponentGui.h"
#include "components/PointLightComponentGui.h"
#include "components/RigidBodyComponentGui.h"
#include "components/TransformComponentGui.h"

#define BIND_COMPONENT_GUI(Component) \
    { sl::core::typeIndex<sl::scene::components::Component>(), std::make_shared<sl::gui::components::Component##Gui>() }

namespace sl::gui {

ComponentsGui::ComponentsGui() {
    m_componentsGui = {
        BIND_COMPONENT_GUI(DirectionalLightComponent),
        BIND_COMPONENT_GUI(MaterialComponent),
        BIND_COMPONENT_GUI(MeshRendererComponent),
        BIND_COMPONENT_GUI(ModelComponent),
        BIND_COMPONENT_GUI(ParticleEffectComponent),
        BIND_COMPONENT_GUI(PointLightComponent),
        BIND_COMPONENT_GUI(TransformComponent),
        BIND_COMPONENT_GUI(RigidBodyComponent)
    };
}

void ComponentsGui::renderComponentGui(std::type_index index, ecs::Component& component,
    asset::AssetManager& assetManager, ecs::Entity& entity) {

    if (m_componentsGui.contains(index))
        m_componentsGui.at(index)->renderComponentGui(component, assetManager, entity);
}

}
