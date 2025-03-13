#include "SceneView.hh"

#include "Events.hh"

#include <starlight/renderer/MeshComposite.hh>
#include <starlight/app/factories/MeshFactory.hh>
#include <starlight/app/factories/MaterialFactory.hh>
#include <starlight/app/factories/TextureFactory.hh>

namespace sle {

SceneView::SceneView(Data& data) : m_data(data), m_tabMenu("Scene") {
    m_tabMenu
      .addTab(ICON_FA_CODE_BRANCH "  Entities Tree", [&]() { renderEntitiesTab(); })
      .addTab(ICON_FA_CLOUD "  Skybox", [&]() { renderSkyboxTab(); })
      .addTab(ICON_FA_CAMERA "  Camera", [&]() { renderCameraTab(); });
}

void SceneView::render() { m_tabMenu.render(); }

void renderEntityInspector(
  sl::Entity& entity, SceneView::EntityData& entityData, ComponentViews& views
);

void SceneView::renderEntitiesTab() {
    auto& scene = *m_data.scene;

    if (sl::ui::button("Add Entity", sl::ui::parentWidth)) {
        auto& entity = scene.addEntity();
        editorWriteInfo("New entity added: {}/{}", entity.id, entity.name);
    }

    sl::ui::separator();
    sl::ui::treeNode(
      "Root",
      [&]() {
          scene.forEachEntity([&](sl::Entity& entity) {
              auto flags =
                ImGuiTreeNodeFlags_OpenOnDoubleClick
                | ImGuiTreeNodeFlags_DefaultOpen;

              if (m_data.selectedEntity != nullptr
                  && m_data.selectedEntity->id == entity.id)
                  flags |= ImGuiTreeNodeFlags_Selected;
              sl::ui::treeNode(
                entity.name,
                [&]() {
                    // TODO: display child entitites
                },
                flags
              );
              if (sl::ui::wasItemClicked()) {
                  m_data.selectedEntity    = &entity;
                  m_data.inspectorCallback = [&]() {
                      renderEntityInspector(
                        entity, m_entitiesData[entity.id], m_componentViews
                      );
                  };
              }
          });
      },
      ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen
    );
}

#define ADD_COMPONENT(Component, ...)                            \
    if (entity.hasComponent<Component>()) {                      \
        editorWriteWarn("Component already added, skipping..."); \
    } else {                                                     \
        editorWriteDebug("Adding component: {}", #Component);    \
        entity.addComponent<Component>(__VA_ARGS__);             \
    }

void renderEntityInspector(
  sl::Entity& entity, SceneView::EntityData& entityData, ComponentViews& views
) {
    static std::vector<const char*> componentNames = {
        "MeshComposite", "PointLight", "DirectionalLight"
    };

    entityData.nameBuffer = entity.name;

    sl::ui::namedScope(entity.name, [&]() {
        if (ImGui::InputText(
              "##", &entityData.nameBuffer, ImGuiInputTextFlags_EnterReturnsTrue
            )) {
            editorWriteDebug("Entity name changed to: {}", entityData.nameBuffer);
            entity.name = entityData.nameBuffer;
        }
        sl::ui::sameLine();

        if (sl::ui::button("Remove", sl::ui::parentWidth)) {
        }

        sl::ui::separator();

        ImGui::Combo(
          "##combo2", &entityData.selectedComponentIndex, componentNames.data(),
          componentNames.size()
        );

        sl::ui::sameLine();

        if (sl::ui::button("Add", sl::ui::parentWidth)) {
            editorWriteDebug(
              "Add component clicked: {}/{}", entity.name,
              entityData.selectedComponentIndex
            );

            if (entityData.selectedComponentIndex == 0) {
                ADD_COMPONENT(
                  sl::MeshComposite, sl::MeshFactory::get().getCube(),
                  sl::MaterialFactory::get().getDefault()
                );
            } else if (entityData.selectedComponentIndex == 1) {
                ADD_COMPONENT(sl::PointLight);
            } else if (entityData.selectedComponentIndex == 2) {
                ADD_COMPONENT(sl::DirectionalLight);
            }
        }

        for (const auto& componentType : entity.getComponentTypes()) {
            sl::ui::separator();
            sl::ui::namedScope(componentType.name(), [&]() {
                views.render(componentType, entity.getComponent(componentType));
            });
        }
    });
}

void SceneView::renderCameraTab() {}

void SceneView::renderSkyboxTab() {}

}  // namespace sle
