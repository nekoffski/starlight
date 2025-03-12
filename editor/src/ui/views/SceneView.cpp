#include "SceneView.hh"

#include <starlight/event/EventProxy.hh>
#include <starlight/renderer/MeshComposite.hh>
#include <starlight/app/factories/MeshFactory.hh>
#include <starlight/app/factories/MaterialFactory.hh>
#include <starlight/app/factories/TextureFactory.hh>

#include "Events.hh"

namespace sle {

SceneView::SceneView(Data& data) : m_data(data), m_tabMenu("Scene") {
    m_tabMenu
      .addTab(ICON_FA_CODE_BRANCH "  Entities Tree", [&]() { renderEntitiesTab(); })
      .addTab(ICON_FA_CLOUD "  Skybox", [&]() { renderSkyboxTab(); })
      .addTab(ICON_FA_CAMERA "  Camera", [&]() { renderCameraTab(); });
}

void SceneView::render() { m_tabMenu.render(); }

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
                  m_data.inspectorCallback = [&]() { renderEntityInspector(); };
              }
          });
      },
      ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen
    );
}

#define ADD_COMPONENT(Component, ...)                                \
    if (m_data.selectedEntity->hasComponent<Component>()) {          \
        editorWriteWarn("Component already added, skipping...");     \
    } else {                                                         \
        m_data.selectedEntity->addComponent<Component>(__VA_ARGS__); \
    }

void SceneView::renderEntityInspector() {
    static std::vector<const char*> componentNames = {
        "MeshComposite", "PointLight", "DirectionalLight"
    };

    m_entityData.nameBuffer = m_data.selectedEntity->name;

    sl::ui::namedScope(m_data.selectedEntity->name, [&]() {
        if (ImGui::InputText(
              "##", &m_entityData.nameBuffer, ImGuiInputTextFlags_EnterReturnsTrue
            )) {
            editorWriteDebug("Entity name changed to: {}", m_entityData.nameBuffer);
            m_data.selectedEntity->name = m_entityData.nameBuffer;
        }
        sl::ui::separator();

        ImGui::Combo(
          "##combo2", &m_entityData.selectedComponentIndex, componentNames.data(),
          componentNames.size()
        );

        sl::ui::sameLine();

        if (sl::ui::button("Add Component", sl::ui::parentWidth)) {
            editorWriteDebug(
              "Add component clicked: {}/{}", m_data.selectedEntity->name,
              m_entityData.selectedComponentIndex
            );

            if (m_entityData.selectedComponentIndex == 0) {
                ADD_COMPONENT(
                  sl::MeshComposite, sl::MeshFactory::get().getCube(),
                  sl::MaterialFactory::get().getDefault()
                );
            } else if (m_entityData.selectedComponentIndex == 1) {
                ADD_COMPONENT(sl::PointLight);
            } else if (m_entityData.selectedComponentIndex == 2) {
                ADD_COMPONENT(sl::DirectionalLight);
            }
        }

        if (sl::ui::button("Remove Entity", sl::ui::parentWidth)) {
        }

        for (auto componentType : m_data.selectedEntity->getComponentTypes()) {
            sl::ui::separator();
            m_componentViews.render(
              componentType, m_data.selectedEntity->getComponent(componentType)
            );
        }
    });
}

void SceneView::renderCameraTab() {}

void SceneView::renderSkyboxTab() {}

}  // namespace sle
