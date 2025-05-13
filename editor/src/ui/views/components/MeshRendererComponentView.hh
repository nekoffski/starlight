#pragma once

#include "ComponentView.hh"

#include "starlight/app/scene/components/MeshRendererComponent.hh"

namespace sle {

struct MeshRendererComponentView : ComponentViewBase<sl::MeshRendererComponent> {
    using ComponentViewBase::ComponentViewBase;

    void render(sl::MeshRendererComponent& component) override;
};

}  // namespace sle
