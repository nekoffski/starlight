#include "SceneView.hh"

#include "Events.hh"

#include <starlight/app/factories/MeshFactory.hh>
#include <starlight/app/factories/MaterialFactory.hh>
#include <starlight/app/factories/TextureFactory.hh>
#include <starlight/app/factories/ModelFactory.hh>
#include <starlight/app/scene/Components.hh>
#include <starlight/core/math/Utils.hh>
#include <starlight/physx/Ray.hh>

#include <ImGuizmo.h>

namespace sle {

SceneView::SceneView(Widget::State& state
) : Widget(state), m_tabMenu("Scene"), m_eventSentinel(sl::EventProxy::get()) {
    m_tabMenu
      .addTab(ICON_FA_CODE_BRANCH "  Entities Tree", [&]() { renderEntitiesTab(); })
      .addTab(ICON_FA_CLOUD "  Skybox", [&]() { renderSkyboxTab(); })
      .addTab(ICON_FA_CAMERA "  Camera", [&]() { renderCameraTab(); });

    m_eventSentinel.add<sl::MouseEvent>([&](auto& event) {
        if (event.action == sl::MouseAction::press
            && event.button == SL_MOUSE_BUTTON_1) {
            traceEntity(event.position);
        }
    });
}

void SceneView::render() {
    m_tabMenu.render();

    if (auto entity = getSeletedEntity();
        entity && entity->has<sl::TransformComponent>()) {
        auto& transform = entity->get<sl::TransformComponent>()->data().getLocal();

        auto& camera = getCamera();

        const auto& view       = camera.getViewMatrix();
        const auto& projection = camera.getProjectionMatrix();

        auto coords   = getRenderPreviewCoords();
        auto viewport = getBiasedViewport();

        ImGuizmo::SetRect(coords.x, coords.y, viewport.x, viewport.y);
        ImGuizmo::Manipulate(
          sl::math::value_ptr(view), sl::math::value_ptr(projection),
          ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, sl::math::value_ptr(transform),
          nullptr, nullptr
        );
    }
}

void renderEntityInspector(
  sl::Entity& entity, SceneView::EntityData& entityData, ComponentViews& views
);

void SceneView::setSelectedEntity(sl::Entity& entity) {
    Widget::setSelectedEntity(entity, [&]() {
        renderEntityInspector(entity, m_entitiesData[entity.id], m_componentViews);
    });
}

void SceneView::traceEntity(const sl::Vec2<sl::f32>& mousePosition) {
    const auto biasedCoordinates = getBiasedCoords(mousePosition);
    const auto biasedViewport    = getBiasedViewport();

    auto& camera = getCamera();

    const auto direction = sl::deproject(
      biasedCoordinates, biasedViewport, camera.getInvProjectionMatrix(),
      camera.getInvViewMatrix()
    );

    sl::Ray ray{ camera.getPosition(), direction };

    sl::Entity* hitEntity = nullptr;
    auto closestHit       = sl::max<sl::f32>();

    getScene().forEach<sl::ModelComponent>([&](auto& component) {
        auto& boundingVolume = component->getBoundingVolume();

        if (auto intersection = boundingVolume.intersects(ray); intersection) {
            if (intersection->min < closestHit) {
                closestHit = intersection->min;
                hitEntity  = &component.getEntity();
            }
        }
    });

    if (hitEntity)
        setSelectedEntity(*hitEntity);
    else
        resetSelectedEntity();
}

void SceneView::renderEntitiesTab() {
    auto& scene = getScene();

    if (sl::ui::button("Add Entity", sl::ui::parentWidth)) {
        auto& entity = scene.addEntity();
        editorWriteInfo("New entity added: {}/{}", entity.id, entity.name);
    }

    sl::ui::separator();
    sl::ui::treeNode(
      "Root",
      [&]() {
          scene.forEach([&](sl::Entity& entity) {
              auto flags =
                ImGuiTreeNodeFlags_OpenOnDoubleClick
                | ImGuiTreeNodeFlags_DefaultOpen;

              auto selectedEntity = getSeletedEntity();
              if (selectedEntity != nullptr && selectedEntity->id == entity.id)
                  flags |= ImGuiTreeNodeFlags_Selected;
              sl::ui::treeNode(
                entity.name,
                [&]() {
                    // TODO: display child entitites
                },
                flags
              );
              if (sl::ui::wasItemClicked()) setSelectedEntity(entity);
          });
      },
      ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen
    );
}

#define ADD_COMPONENT(Component, ...)                            \
    if (entity.has<Component>()) {                               \
        editorWriteWarn("Component already added, skipping..."); \
    } else {                                                     \
        editorWriteDebug("Adding component: {}", #Component);    \
        entity.add<Component>(__VA_ARGS__);                      \
    }

void renderEntityInspector(
  sl::Entity& entity, SceneView::EntityData& entityData, ComponentViews& views
) {
    static std::vector<const char*> componentNames = {
        "Model", "PointLight", "DirectionalLight", "Transform"
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
                  sl::ModelComponent, sl::ModelFactory::get().getDefault()
                );
            } else if (entityData.selectedComponentIndex == 1) {
                ADD_COMPONENT(sl::PointLightComponent);
            } else if (entityData.selectedComponentIndex == 2) {
                ADD_COMPONENT(sl::DirectionalLightComponent);
            } else if (entityData.selectedComponentIndex == 3) {
                ADD_COMPONENT(sl::TransformComponent);
            }
        }

        for (const auto& componentType : entity.getComponentTypes()) {
            sl::ui::separator();
            sl::ui::namedScope(componentType.name(), [&]() {
                views.render(componentType, entity.get(componentType));
            });
        }
    });
}

void SceneView::renderCameraTab() {}

void SceneView::renderSkyboxTab() {}

}  // namespace sle
