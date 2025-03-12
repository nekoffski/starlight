#pragma once

// #include <optional>

#include <starlight/ui/UI.hh>
// #include <starlight/app/scene/Entity.hh>
// #include <starlight/app/scene/Scene.hh>
// #include <starlight/event/EventHandlerSentinel.hh>
// #include <starlight/renderer/RenderGraph.hh>

// #include <starlight/renderer/MeshComposite.hh>
// #include <starlight/renderer/light/PointLight.hh>
// #include <starlight/renderer/light/DirectionalLight.hh>

#include "Console.hh"
#include "ui/Data.hh"

namespace sle {

class PropertiesView {
    //     class EntityTab {
    //         struct Data {
    //             sl::Entity* selectedEntity = nullptr;
    //             std::string nameBuffer;
    //             int selectedComponentIndex = 0;
    //         };

    //     public:
    //         explicit EntityTab();

    //         void render();

    //     private:
    //         void renderEntityUI();

    //         sl::EventHandlerSentinel m_eventSentinel;
    //         std::optional<events::SetComponentUICallback::Callback>
    //         m_componentCallback; Data m_data;
    //     };

    //     class ResourceTab {
    //     public:
    //         explicit ResourceTab();
    //         void render();

    //     private:
    //         sl::EventHandlerSentinel m_eventSentinel;
    //         std::optional<events::SetResourceUICallback::Callback>
    //         m_resourceCallback;
    //     };

    //     class RendererTab {
    //     public:
    //         explicit RendererTab(sl::RenderGraph* renderGraph);

    //         void render();
    //         void setRenderGraph(sl::RenderGraph& renderGraph);

    //     private:
    //         sl::RenderGraph* m_renderGraph;
    //     };

public:
    explicit PropertiesView(Data& data);
    void render();

private:
    Data& m_data;
    sl::ui::TabMenu m_tabMenu;

    void renderInspectorTab();
    void renderRendererTab();
};

}  // namespace sle
