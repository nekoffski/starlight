#include "DirectionalLightComponentView.hh"

#include <starlight/math/Utils.hh>

namespace sle {

void DirectionalLightComponentView::render(sl::DirectionalLightComponent& c) {
    auto& component = c.data();

    sl::treeNode(
      ICON_FA_SUN "  DirectionalLight",
      [&]() {
          sl::text("Direction:");
          sl::immediateSlider(
            "##Direction", component.direction, { -1.0f, 1.0f, 0.02f }
          );
          sl::text("Color:");
          ImGui::ColorEdit4("##Color", sl::math::value_ptr(component.color));
      },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

}  // namespace sle
