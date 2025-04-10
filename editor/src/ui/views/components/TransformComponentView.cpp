#include "TransformComponentView.hh"

#include <starlight/core/math/Utils.hh>

#include <ImGuizmo.h>

namespace sle {

void TransformComponentView::render(sl::TransformComponent& c) {
    auto& component = c.data();
    sl::treeNode(
      ICON_FA_ARROWS_ALT "  Transform",
      [&]() {
          auto& camera  = getCamera();
          auto position = component.getPosition();
          if (sl::immediateSlider("##Position", position, { -10.0f, 10.0f, 0.01f }))
              component.setPosition(position);
          sl::sameLine();
          sl::text("Position");

          auto originalEuler = sl::math::degrees(component.getEuler());
          auto euler         = originalEuler;
          if (sl::immediateSlider("##Rotation", euler, { -180.0f, 180.0f, 0.25f })) {
              auto diff = euler - originalEuler;
              component.rotate(sl::math::radians(diff));
          }

          sl::sameLine();
          sl::text("Rotation");

          auto scale = component.getScale();
          if (sl::immediateSlider("##Scale", scale, { -5.0f, 5.0f, 0.01f }))
              component.setScale(scale);
          sl::sameLine();
          sl::text("Scale");

          if (isGizmoEnabled()) {
              auto matrix = component.getLocal();

              const auto& view       = camera.getViewMatrix();
              const auto& projection = camera.getProjectionMatrix();

              auto coords   = getRenderPreviewCoords();
              auto viewport = getBiasedViewport();

              ImGuizmo::SetRect(coords.x, coords.y, viewport.x, viewport.y);

              const auto changed = ImGuizmo::Manipulate(
                sl::math::value_ptr(view), sl::math::value_ptr(projection),
                getGizmoOperation(), getGizmoMode(), sl::math::value_ptr(matrix),
                nullptr, nullptr
              );

              if (changed) {
                  auto euler      = component.getEuler();
                  auto components = sl::decomposeTransformation(matrix);

                  component.setPosition(components.translation);
                  component.rotate(components.euler - euler);
                  component.setScale(components.scale);
              }
          }
      },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

}  // namespace sle
