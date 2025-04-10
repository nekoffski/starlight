#include "PointLightComponentView.hh"

namespace sle {

void PointLightComponentView::render(sl::PointLightComponent& c) {
    auto& component = c.data();
    sl::treeNode(
      ICON_FA_LIGHTBULB "  PointLight",
      [&]() {
          sl::text("Attenuation:");
          auto attenuation = component.getAttenuation();
          if (sl::immediateSlider(
                "##Attenuation", attenuation, { -10.0f, 10.0f, 0.01f }
              ))
              component.setAttenuation(attenuation);

          sl::text("Color:");
          ImGui::ColorEdit4("##Color", sl::math::value_ptr(component.color));
      },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

}  // namespace sle
