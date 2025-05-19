#include "MeshRendererComponentView.hh"

#include <starlight/app/factories/MeshFactory.hh>
#include <starlight/app/factories/MaterialFactory.hh>
#include <starlight/core/TaskQueue.hh>

namespace sle {

void MeshRendererComponentView::render(sl::MeshRendererComponent& c) {
    auto& component = c.data();
    sl::treeNode(
      ICON_FA_CUBE "  Mesh Renderer",
      [&]() {
          sl::text("Mesh: ");
          auto meshes                 = sl::MeshFactory::get().getValues();
          const auto& currentMeshName = component.mesh->getName();
          sl::immediateCombo("##MeshCombo", currentMeshName, [&]() {
              for (auto& mesh : meshes) {
                  bool selected = mesh->getName() == currentMeshName;
                  if (ImGui::Selectable(mesh->getName().c_str(), selected)) {
                      sl::TaskQueue::get().callPostFrame([&]() {
                          component.mesh = mesh;
                      });
                  }
                  if (selected) ImGui::SetItemDefaultFocus();
              }
          });

          sl::text("Material: ");
          auto& materials                 = getResources(Resource::Type::material);
          const auto& currentMaterialName = component.material->getName();
          sl::immediateCombo("##MaterialCombo", currentMaterialName, [&]() {
              for (auto& materialName : materials) {
                  bool selected = materialName == currentMaterialName;
                  if (ImGui::Selectable(materialName.c_str(), selected)) {
                      sl::TaskQueue::get().callPostFrame([&]() {
                          component.material =
                            sl::MaterialFactory::get().load(materialName);
                      });
                  }
                  if (selected) ImGui::SetItemDefaultFocus();
              }
          });
      },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

}  // namespace sle
