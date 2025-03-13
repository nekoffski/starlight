#include "ComponentViews.hh"

#include <starlight/renderer/light/DirectionalLight.hh>
#include <starlight/renderer/light/PointLight.hh>
#include <starlight/ui/UI.hh>

namespace sle {

// void MeshCompositeUI::renderInstanceUI(sl::Transform& instance) {
//     auto position = instance.getPosition();
//     if (sl::ui::slider("Position", position, { -10.0f, 10.0f, 0.01f })) {
//         instance.setPosition(position);
//     }
//     auto scale = instance.getScale();
//     if (sl::ui::slider("Scale", scale, { -5.0f, 5.0f, 0.01f })) {
//         instance.setScale(scale);
//     }
// }

// void MeshCompositeUI::renderNodeUI(sl::MeshComposite::Node& node) {
//     // sl::ui::combo(
//     //   "Mesh", node.mesh->name, sl::MeshFactory::get().getAll(),
//     //   [&](auto& mesh) { node.mesh = mesh; }
//     // );
//     // sl::ui::combo(
//     //   "Material", node.material->name, sl::MaterialFactory::get().getAll(),
//     //   [&](auto& material) { node.material = material; }
//     // );
//     if (sl::ui::button("Add Instance")) node.addInstance();
// }

// void MeshCompositeUI::renderMeshCompositeUI([[maybe_unused]] sl::MeshComposite&
// mesh ) {}

// bool MeshCompositeUI::renderSceneNode(sl::MeshComposite& component) {
//     const auto treeFlags =
//       ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
//     bool clicked = false;

//     auto uiOnClick = [&](auto&& callback) {
//         if (sl::ui::wasItemClicked()) {
//             clicked = true;
//             sl::EventProxy::get().emit<events::SetComponentUICallback>(
//               std::move(callback)
//             );
//         }
//     };

//     sl::ui::treeNode(
//       ICON_FA_CUBES "  MeshComposite",
//       [&]() {
//           uiOnClick([&]() { renderMeshCompositeUI(component); });

//           component.traverse([&](auto& node) {
//               sl::ui::treeNode(
//                 fmt::format("{}  {}", ICON_FA_CUBE, node.name),
//                 [&]() {
//                     uiOnClick([&]() { renderNodeUI(node); });

//                     auto instances = node.getInstances();
//                     for (sl::u64 i = 0; i < instances.size(); ++i) {
//                         auto& instance          = instances[i];
//                         const auto instanceName = fmt::format("Instance_{}", i);
//                         ImGui::BulletText("%s", instanceName.c_str());
//                         uiOnClick([&]() { renderInstanceUI(instance); });
//                     }
//                 },
//                 treeFlags
//               );
//           });
//       },
//       treeFlags
//     );

//     return clicked;
// }

void renderPointLight(sl::PointLight& component) {
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

void renderDirectionalLight(sl::DirectionalLight& component) {
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

ComponentViews::ComponentViews() {
    registerView<sl::DirectionalLight>(renderDirectionalLight);
    registerView<sl::PointLight>(renderPointLight);
}

void ComponentViews::render(std::type_index index, void* component) {
    if (auto it = m_views.find(index); it != m_views.end()) it->second(component);
}

}  // namespace sle
