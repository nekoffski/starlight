#pragma once

#include <unordered_map>

#include <starlight/ui/UI.hh>
#include <starlight/event/EventHandlerSentinel.hh>

#include "Console.hh"
#include "ComponentViews.hh"

#include "ui/Widget.hh"

namespace sle {

class SceneView : public Widget {
public:
    struct EntityData {
        std::string nameBuffer;
        sl::i32 selectedComponentIndex;
    };

    explicit SceneView(Widget::State& state);

    void render();

private:
    sl::TabMenu m_tabMenu;
    EntityData m_entityData;
    ComponentViews m_componentViews;

    std::unordered_map<sl::u64, EntityData> m_entitiesData;
    sl::EventHandlerSentinel m_eventSentinel;

    void setSelectedEntity(sl::Entity& entity);

    void traceEntity(const sl::Vec2<sl::f32>& mousePosition);

    void renderEntitiesTab();
    void renderSkyboxTab();
};

}  // namespace sle
