#pragma once

#include <starlight/ui/UI.hh>

#include "Console.hh"
#include "ComponentViews.hh"
#include "ui/Data.hh"

namespace sle {

class SceneView {
    struct EntityData {
        std::string nameBuffer;
        sl::i32 selectedComponentIndex;
    };

public:
    explicit SceneView(Data& data);

    void render();

private:
    Data& m_data;
    EntityData m_entityData;
    ComponentViews m_componentViews;

    sl::ui::TabMenu m_tabMenu;

    void renderEntitiesTab();
    void renderSkyboxTab();
    void renderCameraTab();

    void renderEntityInspector();
};

}  // namespace sle
