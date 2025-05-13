#pragma once

#include "Config.hh"

#include <functional>
#include <optional>
#include <unordered_map>

#include <starlight/ui/UI.hh>

#include <starlight/app/scene/Scene.hh>
#include <starlight/app/scene/Entity.hh>
#include <starlight/renderer/RenderGraph.hh>
#include <starlight/renderer/gpu/Texture.hh>
#include <starlight/renderer/camera/Camera.hh>
#include <starlight/ui/widgets/Image.hh>

#include "Config.hh"
#include "ResourceType.hh"

namespace sle {

class Widget {
    using Callback    = std::function<void()>;
    using OptCallback = std::optional<Callback>;

public:
    class State {
        friend class Widget;

    public:
        explicit State(
          const sl::Vec2<sl::u32>& viewport, const Config& config, sl::Scene& scene,
          sl::RenderGraph& renderGraph, sl::Camera& camera
        );

        void setViewport(const sl::Vec2<sl::u32>& viewport);
        void setScene(sl::Scene& scene);
        void setRenderGraph(sl::RenderGraph& renderGraph);
        void setCamera(sl::Camera& camera);

        bool centerOnSelectedEntity;

    private:
        Config m_config;
        sl::Vec2<sl::u32> m_viewport;
        sl::Scene* m_scene;
        sl::RenderGraph* m_renderGraph;
        sl::Camera* m_camera;
        sl::Entity* m_selectedEntity;

        OptCallback m_inspectorCallback;
        std::unordered_map<sl::u64, kstd::UniquePtr<sl::ImageHandle>> m_images;

        ImGuizmo::MODE m_gizmoMode;
        ImGuizmo::OPERATION m_gizmoOperation;
        bool m_gizmoEnabled;

        std::unordered_map<ResourceType, std::vector<std::string>> m_resources;
    };

    explicit Widget(State& state);

    State& getState();

    sl::RenderGraph& getRenderGraph();
    sl::Scene& getScene();
    sl::Camera& getCamera();
    const Config& getConfig();
    ImGuizmo::MODE& getGizmoMode();
    ImGuizmo::OPERATION& getGizmoOperation();
    bool& isGizmoEnabled();

    const sl::Vec2<sl::u32>& getViewport();

    sl::Entity* getSeletedEntity();
    void setSelectedEntity(sl::Entity& entity, Callback&& callback);
    void setInspectorCallback(Callback&& callback);
    void resetSelectedEntity();

    sl::Vec2<sl::f32> getBiasedCoords(
      const sl::Vec2<sl::f32>& coords = sl::Vec2<sl::f32>{ 0.0f }
    );
    sl::Vec2<sl::f32> getRenderPreviewCoords();
    sl::Vec2<sl::u32> getBiasedViewport();

    OptCallback& getInspectorCallback();

    void showImage(sl::Texture& texture, sl::f32 width);

    std::vector<std::string>& getResources(ResourceType type);
    void addResource(ResourceType type, const std::string& name);
    void resetResources();
    void resetResources(ResourceType);

private:
    State& m_state;
};

}  // namespace sle
