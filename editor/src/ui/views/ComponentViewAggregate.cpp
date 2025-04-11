#include "ComponentViewAggregate.hh"

#include <starlight/app/scene/Components.hh>
#include <starlight/ui/UI.hh>

#include "components/DirectionalLightComponentView.hh"
#include "components/PointLightComponentView.hh"
#include "components/TransformComponentView.hh"
#include "components/MeshComponentView.hh"

namespace sle {

ComponentViewAggregate::ComponentViewAggregate(Widget::State& state)
    : Widget(state) {
    registerView<sl::DirectionalLightComponent, DirectionalLightComponentView>();
    registerView<sl::PointLightComponent, PointLightComponentView>();
    registerView<sl::TransformComponent, TransformComponentView>();
    registerView<sl::MeshComponent, MeshComponentView>();
}

void ComponentViewAggregate::render(std::type_index index, void* component) {
    if (auto it = m_views.find(index); it != m_views.end())
        it->second->render(component);
}

}  // namespace sle
