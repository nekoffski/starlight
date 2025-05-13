#include "ComponentViewAggregate.hh"

#include <starlight/app/scene/Components.hh>
#include <starlight/ui/UI.hh>

#include "components/DirectionalLightComponentView.hh"
#include "components/PointLightComponentView.hh"
#include "components/TransformComponentView.hh"
#include "components/MeshRendererComponentView.hh"
#include "components/BoundingVolumeComponentView.hh"

namespace sle {

ComponentViewAggregate::ComponentViewAggregate(Widget::State& state)
    : Widget(state) {
    registerView<DirectionalLightComponentView>();
    registerView<PointLightComponentView>();
    registerView<TransformComponentView>();
    registerView<MeshRendererComponentView>();
    registerView<BoundingVolumeComponentView>();
}

void ComponentViewAggregate::render(std::type_index index, void* component) {
    if (auto it = m_views.find(index); it != m_views.end())
        it->second->render(component);
}

}  // namespace sle
