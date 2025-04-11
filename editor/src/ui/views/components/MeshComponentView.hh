#pragma once

#include "ComponentView.hh"

#include "starlight/app/scene/components/MeshComponent.hh"

namespace sle {

struct MeshComponentView : ComponentViewBase<sl::MeshComponent> {
    using ComponentViewBase::ComponentViewBase;

    void render(sl::MeshComponent& component) override;
};

}  // namespace sle
