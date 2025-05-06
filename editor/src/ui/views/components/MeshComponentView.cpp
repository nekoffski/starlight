#include "MeshComponentView.hh"

#include "starlight/app/factories/MeshFactory.hh"

namespace sle {

void MeshComponentView::render(sl::MeshComponent& c) {
    auto& component = c.data();
    sl::treeNode(
      ICON_FA_CAR_SIDE "  Mesh",
      [&]() {
          auto meshes = sl::MeshFactory::get().getValues();
          sl::immediateCombo(
            "Mesh", component.mesh->getName(), meshes,
            [&](auto& mesh) { component.mesh = mesh; }
          );
      },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

}  // namespace sle
