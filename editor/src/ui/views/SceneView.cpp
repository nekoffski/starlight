#include "SceneView.hh"

#include "Events.hh"

#include <starlight/app/factories/MeshFactory.hh>
#include <starlight/app/factories/MaterialFactory.hh>
#include <starlight/app/factories/TextureFactory.hh>
#include <starlight/app/factories/SkyboxFactory.hh>
#include <starlight/app/scene/Components.hh>
#include <starlight/math/Utils.hh>
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
        renderEntityInspector(
          entity, m_entitiesData[entity.getId()], m_componentViews
        );
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
        editorWriteInfo("New entity added: {}/{}", entity.getId(), entity.getName());
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
              if (selectedEntity != nullptr
                  && selectedEntity->getId() == entity.getId())
                  flags |= ImGuiTreeNodeFlags_Selected;
              sl::treeNode(
                fmt::format("{}  {}", ICON_FA_CUBE, entity.getName()),
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

#define ADD_COMPONENT(Index, Component, ...)                         \
    if (entityData.selectedComponentIndex == Index) {                \
        if (entity.has<Component>()) {                               \
            editorWriteWarn("Component already added, skipping..."); \
        } else {                                                     \
            editorWriteDebug("Adding component: {}", #Component);    \
            entity.add<Component>(__VA_ARGS__);                      \
        }                                                            \
    }

void renderEntityInspector(
  sl::Entity& entity, SceneView::EntityData& entityData,
  ComponentViewAggregate& views
) {
    static std::vector<const char*> componentNames = {
        "Transform", "Point Light", "Directional Light", "Mesh Renderer"
    };

    entityData.nameBuffer = entity.getName();
    sl::namedScope(entity.getName(), [&]() {
        if (ImGui::InputText(
              "##", &entityData.nameBuffer, ImGuiInputTextFlags_EnterReturnsTrue
            )) {
            editorWriteDebug("Entity name changed to: {}", entityData.nameBuffer);
            entity.setName(entityData.nameBuffer);
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
              "Add component clicked: {}/{}", entity.getName(),
              entityData.selectedComponentIndex
            );

            ADD_COMPONENT(0u, sl::TransformComponent);
            ADD_COMPONENT(1u, sl::PointLightComponent);
            ADD_COMPONENT(2u, sl::DirectionalLightComponent);
            ADD_COMPONENT(3u, sl::MeshRendererComponent);
        }

        for (const auto& componentType : entity.getComponentTypes()) {
            sl::separator();
            sl::namedScope(componentType.name(), [&]() {
                views.render(componentType, entity.get(componentType));
            });
            sl::lineBreak();
        }
    });
}

void SceneView::renderSkyboxTab() {
    // TODO: FIXME, create a proper CubemapFactory

    std::vector<std::string> skyboxes = { "None" };
    auto textures                     = getResources(Resource::Type::texture);
    for (const auto& t : textures)
        if (t.ends_with("_u")) skyboxes.push_back(t.substr(0, t.size() - 2u));

    auto& scene = getScene();

    std::string skyboxName = "None";
    if (auto skybox = scene.getSkybox(); skybox) skyboxName = skybox->getName();

    sl::immediateCombo("##SkyboxCombo", skyboxName, [&]() {
        for (auto& skybox : skyboxes) {
            bool selected = skyboxName == skybox;
            if (ImGui::Selectable(skybox.c_str(), selected)) {
                if (skybox == "None")
                    scene.resetSkybox();
                else
                    scene.setSkybox(sl::SkyboxFactory::get().load(skybox));
            }
            if (selected) ImGui::SetItemDefaultFocus();
        }
    });
}

}  // namespace sle
