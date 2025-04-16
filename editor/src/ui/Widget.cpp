#include "Widget.hh"

#include <starlight/core/Function.hh>

namespace sle {

Widget::Widget(State& state)
    : m_state(state) {}

sl::Scene& Widget::getScene() { return *m_state.m_scene; }

const sl::Vec2<sl::u32>& Widget::getViewport() { return m_state.m_viewport; }

sl::Entity* Widget::getSeletedEntity() { return m_state.m_selectedEntity; }

void Widget::setSelectedEntity(sl::Entity& entity, Callback&& callback) {
    m_state.m_selectedEntity    = &entity;
    m_state.m_inspectorCallback = std::move(callback);
}

void Widget::setInspectorCallback(Callback&& callback) {
    m_state.m_selectedEntity    = nullptr;
    m_state.m_inspectorCallback = std::move(callback);
}

void Widget::resetSelectedEntity() {
    m_state.m_selectedEntity = nullptr;
    m_state.m_inspectorCallback.reset();
}

sl::Vec2<sl::f32> Widget::getBiasedCoords(const sl::Vec2<sl::f32>& coords) {
    return sl::Vec2<sl::f32>{
        coords.x - m_state.m_config.layoutSizeRatio.x * m_state.m_viewport.x,
        coords.y
    };
}

sl::Vec2<sl::f32> Widget::getRenderPreviewCoords() {
    return sl::Vec2<sl::f32>{
        m_state.m_config.layoutSizeRatio.x * m_state.m_viewport.x,
        0.0f,
    };
}

sl::Vec2<sl::u32> Widget::getBiasedViewport() {
    return sl::Vec2<sl::u32>{
        static_cast<sl::u32>(
          m_state.m_viewport.x * (1.0f - m_state.m_config.layoutSizeRatio.x * 2.0f)
        ),
        static_cast<sl::u32>(
          m_state.m_viewport.y * (1.0f - m_state.m_config.layoutSizeRatio.y)
        )
    };
}

Widget::OptCallback& Widget::getInspectorCallback() {
    return m_state.m_inspectorCallback;
}

void Widget::showImage(sl::Texture& texture, sl::f32 width) {
    auto [it, _] = m_state.m_images.try_emplace(
      texture.id,
      sl::lazyEvaluate([&] { return sl::ImageHandle::createHandle(&texture); })
    );
    it->second->show({ width, width }, { 0, 0 }, { 1.0f, 1.0f });
}

sl::Camera& Widget::getCamera() { return *m_state.m_camera; }

ImGuizmo::MODE& Widget::getGizmoMode() { return m_state.m_gizmoMode; }

const Config& Widget::getConfig() { return m_state.m_config; }

ImGuizmo::OPERATION& Widget::getGizmoOperation() { return m_state.m_gizmoOperation; }

bool& Widget::isGizmoEnabled() { return m_state.m_gizmoEnabled; }

sl::RenderGraph& Widget::getRenderGraph() { return *m_state.m_renderGraph; }

Widget::State& Widget::getState() { return m_state; }

Widget::State::State(
  const sl::Vec2<sl::u32>& viewport, const Config& config, sl::Scene& scene,
  sl::RenderGraph& renderGraph, sl::Camera& camera
)
    : centerOnSelectedEntity(true)
    , m_config(config)
    , m_viewport(viewport)
    , m_scene(&scene)
    , m_renderGraph(&renderGraph)
    , m_camera(&camera)
    , m_selectedEntity(nullptr)
    , m_gizmoMode(ImGuizmo::LOCAL)
    , m_gizmoOperation(ImGuizmo::TRANSLATE)
    , m_gizmoEnabled(true) {}

void Widget::State::setScene(sl::Scene& scene) { m_scene = &scene; }

void Widget::State::setCamera(sl::Camera& camera) { m_camera = &camera; }

void Widget::State::setRenderGraph(sl::RenderGraph& renderGraph) {
    m_renderGraph = &renderGraph;
}

void Widget::State::setViewport(const sl::Vec2<sl::u32>& viewport) {
    m_viewport = viewport;
}

}  // namespace sle
