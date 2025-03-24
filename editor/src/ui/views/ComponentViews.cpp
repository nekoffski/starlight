#include "ComponentViews.hh"

#include <starlight/app/scene/Components.hh>
#include <starlight/ui/UI.hh>
#include <starlight/core/math/Utils.hh>

namespace sle {

void renderPointLight(sl::PointLightComponent& c) {
    auto& component = c.data();
    sl::ui::treeNode(
      ICON_FA_LIGHTBULB "  PointLight",
      [&]() {
          sl::ui::text("Position:");
          sl::ui::slider("##Position", component.position, { -10.0f, 10.0f, 0.01f });

          sl::ui::text("Attenuation:");
          auto attenuation = component.getAttenuation();
          if (sl::ui::slider("##Attenuation", attenuation, { -10.0f, 10.0f, 0.01f }))
              component.setAttenuation(attenuation);

          sl::ui::text("Color:");
          ImGui::ColorEdit4("##Color", sl::math::value_ptr(component.color));
      },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

void renderDirectionalLight(sl::DirectionalLightComponent& c) {
    auto& component = c.data();
    sl::ui::treeNode(
      ICON_FA_SUN "  DirectionalLight",
      [&]() {
          sl::ui::text("Direction:");
          sl::ui::slider("##Direction", component.direction, { -1.0f, 1.0f, 0.02f });
          sl::ui::text("Color:");
          ImGui::ColorEdit4("##Color", sl::math::value_ptr(component.color));
      },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

void renderModel(sl::ModelComponent& c) {
    auto& component = c.data();
    sl::ui::treeNode(
      ICON_FA_CAR "  Model",
      [&]() {

      },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

void renderTransform(sl::TransformComponent& c) {
    auto& component = c.data();
    sl::ui::treeNode(
      ICON_FA_ARROWS_ALT "  Transform",
      [&]() {
          auto components = sl::decomposeTransformation(component.getLocal());

          if (sl::ui::slider(
                "##Position", components.translation, { -10.0f, 10.0f, 0.01f }
              )) {
              component.setPosition(components.translation);
          }
          sl::ui::sameLine();
          sl::ui::text("Position");

          auto cp = components.euler;

          if (sl::ui::slider(
                "##Rotation", components.euler, { -3.14f, 3.14f, 0.001f }
              )) {
              sl::log::warn("{} - {}", components.euler, cp);
              auto rotation = sl::math::eulerAngleXYZ(
                components.euler.x, components.euler.y, components.euler.z
              );
              component.setRotation(rotation);
          }

          sl::ui::sameLine();
          sl::ui::text("Rotation");

          if (sl::ui::slider("##Scale", components.scale, { -5.0f, 5.0f, 0.01f }))
              component.setScale(components.scale);
          sl::ui::sameLine();
          sl::ui::text("Scale");
      },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

ComponentViews::ComponentViews() {
    registerView<sl::DirectionalLightComponent>(renderDirectionalLight);
    registerView<sl::PointLightComponent>(renderPointLight);
    registerView<sl::ModelComponent>(renderModel);
    registerView<sl::TransformComponent>(renderTransform);
}

void ComponentViews::render(std::type_index index, void* component) {
    if (auto it = m_views.find(index); it != m_views.end()) it->second(component);
}

}  // namespace sle
