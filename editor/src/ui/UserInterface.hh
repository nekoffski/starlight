#pragma once

#include <starlight/ui/UI.hh>
#include <starlight/app/scene/Scene.hh>
#include <starlight/core/memory/Memory.hh>

#include "views/SceneView.hh"
#include "views/PropertiesView.hh"
#include "views/ResourcesView.hh"
#include "Console.hh"
#include "Data.hh"

#include "starlight/app/renderPasses/UIRenderPass.hh"

namespace sle {

class UserInterface : public sl::UI {
public:
    explicit UserInterface(
      const sl::Vec2<sl::u32>& viewport, sl::Scene* scene,
      sl::RenderGraph* renderGraph = nullptr,
      const Config& config         = Config::createDefault()
    );

    void onViewportReisze(const sl::Vec2<sl::u32>& viewport);

    void setRenderGraph(sl::RenderGraph& renderGraph);
    void setScene(sl::Scene& scene);
    void setCamera(sl::Camera& camera);

    void render() override;

    const Config& getConfig() const;

private:
    void createLayout(const sl::Vec2<sl::u32>& viewport);
    void initMenu();
    void initLeftCombo();
    void initBottomCombo();

    sl::EventHandlerSentinel m_eventSentinel;

    sl::Vec2<sl::u32> m_viewport;

    Console m_console;
    Data m_data;

    sl::ui::MainMenuBar m_menu;
    sl::LocalPtr<sl::ui::PanelCombo> m_leftCombo;
    sl::LocalPtr<sl::ui::PanelCombo> m_bottomCombo;

    SceneView m_sceneView;
    PropertiesView m_propertiesView;
    ResourcesView m_resourcesView;
};

}  // namespace sle
