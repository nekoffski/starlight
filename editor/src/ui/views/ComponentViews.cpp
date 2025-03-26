#include "ComponentViews.hh"

#include <starlight/app/scene/Components.hh>
#include <starlight/ui/UI.hh>
#include <starlight/core/math/Utils.hh>

#include <ImGuizmo.h>

namespace sle {

void renderPointLight(sl::PointLightComponent& c, [[maybe_unused]] Widget& widget) {
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

void renderDirectionalLight(
  sl::DirectionalLightComponent& c, [[maybe_unused]] Widget& widget
) {
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

void renderModel(sl::ModelComponent& c, [[maybe_unused]] Widget& widget) {
    auto& component = c.data();
    sl::ui::treeNode(
      ICON_FA_CAR "  Model",
      [&]() {

      },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

void renderTransform(sl::TransformComponent& c, Widget& widget) {
    auto& component = c.data();
    sl::ui::treeNode(
      ICON_FA_ARROWS_ALT "  Transform",
      [&]() {
          auto& camera  = widget.getCamera();
          auto position = component.getPosition();
          if (sl::ui::slider("##Position", position, { -10.0f, 10.0f, 0.01f }))
              component.setPosition(position);
          sl::ui::sameLine();
          sl::ui::text("Position");

          auto originalEuler = sl::math::degrees(component.getEuler());
          auto euler         = originalEuler;
          if (sl::ui::slider("##Rotation", euler, { -180.0f, 180.0f, 0.25f })) {
              auto diff = euler - originalEuler;
              component.rotate(sl::math::radians(diff));
          }

          sl::ui::sameLine();
          sl::ui::text("Rotation");

          auto scale = component.getScale();
          if (sl::ui::slider("##Scale", scale, { -5.0f, 5.0f, 0.01f }))
              component.setScale(scale);
          sl::ui::sameLine();
          sl::ui::text("Scale");

          auto matrix = component.getLocal();

          const auto& view       = camera.getViewMatrix();
          const auto& projection = camera.getProjectionMatrix();

          auto coords   = widget.getRenderPreviewCoords();
          auto viewport = widget.getBiasedViewport();

          ImGuizmo::SetRect(coords.x, coords.y, viewport.x, viewport.y);

          const auto changed = ImGuizmo::Manipulate(
            sl::math::value_ptr(view), sl::math::value_ptr(projection),
            ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, sl::math::value_ptr(matrix),
            nullptr, nullptr
          );

          if (changed) {
              auto euler      = component.getEuler();
              auto components = sl::decomposeTransformation(matrix);

              component.setPosition(components.translation);
              component.rotate(components.euler - euler);
              component.setScale(components.scale);
          }
      },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

ComponentViews::ComponentViews(Widget::State& state) : Widget(state) {
    registerView<sl::DirectionalLightComponent>(renderDirectionalLight);
    registerView<sl::PointLightComponent>(renderPointLight);
    registerView<sl::ModelComponent>(renderModel);
    registerView<sl::TransformComponent>(renderTransform);
}

void ComponentViews::render(std::type_index index, void* component) {
    if (auto it = m_views.find(index); it != m_views.end())
        it->second(component, *this);
}

}  // namespace sle
