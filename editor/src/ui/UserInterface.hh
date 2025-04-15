#pragma once

#include <starlight/ui/UI.hh>
#include <starlight/app/scene/Scene.hh>
#include <starlight/core/memory/Memory.hh>

#include "views/SceneView.hh"
#include "views/PropertiesView.hh"
#include "views/ResourcesView.hh"
#include "views/InspectorView.hh"
#include "Console.hh"
#include "Widget.hh"
#include "Config.hh"

#include "starlight/app/renderPasses/UIRenderPass.hh"

namespace sle {

class UserInterface : public sl::UI {
public:
    explicit UserInterface(
      const sl::Vec2<sl::u32>& viewport, sl::Scene& scene,
      sl::RenderGraph& renderGraph, sl::Camera& camera, const Config& config
    );

    void setRenderGraph(sl::RenderGraph& renderGraph);
    void setScene(sl::Scene& scene);
    void setCamera(sl::Camera& camera);

    void render() override;

    const Config& getConfig() const;

    sl::Vec4<sl::f32> getViewportScale() const;

private:
    void onViewportReisze(const sl::Vec2<sl::u32>& viewport);
    void createLayout(const sl::Vec2<sl::u32>& viewport);
    void initMenu();
    void initLeftCombo();
    void initBottomCombo();

    sl::EventHandlerSentinel m_eventSentinel;

    sl::Vec2<sl::u32> m_viewport;

    Console m_console;
    Config m_config;
    Widget::State m_widgetState;

    sl::MainMenuBar m_menu;
    sl::LocalPtr<sl::PanelCombo> m_leftCombo;
    sl::LocalPtr<sl::PanelCombo> m_bottomCombo;
    sl::LocalPtr<sl::PanelCombo> m_inspectorPanel;

    SceneView m_sceneView;
    PropertiesView m_propertiesView;
    ResourcesView m_resourcesView;
    InspectorView m_inspectorView;
};

}  // namespace sle
