#include "SceneView.hh"

#include "Events.hh"

#include <starlight/app/factories/MeshFactory.hh>
#include <starlight/app/factories/MaterialFactory.hh>
#include <starlight/app/factories/TextureFactory.hh>
#include <starlight/app/scene/Components.hh>
#include <starlight/core/math/Utils.hh>
#include <starlight/physx/Ray.hh>

namespace sle {

SceneView::SceneView(Widget::State& state)
    : Widget(state)
    , m_tabMenu("Scene")
    , m_componentViews(state)
    , m_eventSentinel(sl::EventProxy::get()) {
    m_tabMenu
      .addTab(ICON_FA_CODE_BRANCH "  Entities Tree", [&]() { renderEntitiesTab(); })
      .addTab(ICON_FA_CLOUD "  Skybox", [&]() { renderSkyboxTab(); });

    m_eventSentinel.add<sl::MouseEvent>([&](auto& event) {
        if (event.action == sl::MouseAction::press
            && event.button == SL_MOUSE_BUTTON_1) {
            traceEntity(event.position);
        }
    });
}

void SceneView::render() { m_tabMenu.render(); }

void renderEntityInspector(
  sl::Entity& entity, SceneView::EntityData& entityData,
  ComponentViewAggregate& views
);

void SceneView::setSelectedEntity(sl::Entity& entity) {
    Widget::setSelectedEntity(entity, [&]() {
        renderEntityInspector(entity, m_entitiesData[entity.id], m_componentViews);
    });

    if (getState().centerOnSelectedEntity && entity.has<sl::TransformComponent>()) {
        auto position = entity.get<sl::TransformComponent>()->data().getPosition();
        static constexpr sl::u32 steps = 20u;
        getCamera().lookAt(position, steps);
    }
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

    getScene().forEach<sl::BoundingVolumeComponent>([&](auto& component) {
        auto bv = component->boundingVolume.get();

        if (auto intersection = bv->intersects(ray); intersection) {
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

    if (sl::button("Add Entity", sl::parentWidth)) {
        auto& entity = scene.addEntity();
        editorWriteInfo("New entity added: {}/{}", entity.id, entity.name);
    }

    sl::separator();
    sl::treeNode(
      ICON_FA_PROJECT_DIAGRAM "  Scene",
      [&]() {
          scene.forEach([&](sl::Entity& entity) {
              auto flags =
                ImGuiTreeNodeFlags_OpenOnDoubleClick
                | ImGuiTreeNodeFlags_DefaultOpen;

              auto selectedEntity = getSeletedEntity();
              if (selectedEntity != nullptr && selectedEntity->id == entity.id)
                  flags |= ImGuiTreeNodeFlags_Selected;
              sl::treeNode(
                fmt::format("{}  {}", ICON_FA_CUBE, entity.name),
                [&]() {
                    // TODO: display child entitites
                },
                flags
              );
              if (sl::wasItemClicked()) setSelectedEntity(entity);
          });
      },
      ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen
    );
    sl::treeNode(
      ICON_FA_CUBES "  Prefabs", [&]() {},
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
  sl::Entity& entity, SceneView::EntityData& entityData,
  ComponentViewAggregate& views
) {
    static std::vector<const char*> componentNames = {
        "Model", "PointLight", "DirectionalLight", "Transform"
    };

    entityData.nameBuffer = entity.name;
    sl::namedScope(entity.name, [&]() {
        if (ImGui::InputText(
              "##", &entityData.nameBuffer, ImGuiInputTextFlags_EnterReturnsTrue
            )) {
            editorWriteDebug("Entity name changed to: {}", entityData.nameBuffer);
            entity.name = entityData.nameBuffer;
        }
        sl::sameLine();

        if (sl::button("Remove", sl::parentWidth)) {
        }

        sl::separator();

        ImGui::Combo(
          "##combo2", &entityData.selectedComponentIndex, componentNames.data(),
          componentNames.size()
        );

        sl::sameLine();

        if (sl::button("Add", sl::parentWidth)) {
            editorWriteDebug(
              "Add component clicked: {}/{}", entity.name,
              entityData.selectedComponentIndex
            );

            if (entityData.selectedComponentIndex == 0) {
                // ADD_COMPONENT(
                //   sl::ModelComponent, sl::ModelFactory::get().getDefault()
                // );
            } else if (entityData.selectedComponentIndex == 1) {
                ADD_COMPONENT(sl::PointLightComponent);
            } else if (entityData.selectedComponentIndex == 2) {
                ADD_COMPONENT(sl::DirectionalLightComponent);
            } else if (entityData.selectedComponentIndex == 3) {
                ADD_COMPONENT(sl::TransformComponent);
            }
        }

        for (const auto& componentType : entity.getComponentTypes()) {
            sl::separator();
            sl::namedScope(componentType.name(), [&]() {
                views.render(componentType, entity.get(componentType));
            });
        }
    });
}

void SceneView::renderSkyboxTab() {}

}  // namespace sle
