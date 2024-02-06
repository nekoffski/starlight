#pragma once

#include "starlight/ui/Core.h"

namespace sl::ui {

class Panel {
public:
    struct Properties {
        Vec2f position;
        Vec2f size;
    };

    explicit Panel(
      const std::string& name, const Properties& props, Callback&& callback
    );
    void render();

    void setSize(const Vec2f& size);
    void setPosition(const Vec2f& position);

private:
    std::string m_name;
    Properties m_props;
    Callback m_callback;
};

class PanelCombo {
public:
    struct Properties {
        Vec2f position;
        Vec2f size;
        bool alignWithMainMenuBar;
    };

    explicit PanelCombo(const std::string& name, const Properties& props);

    void render();

    PanelCombo& addPanel(const std::string& name, Callback&& callback);

    void setSize(const Vec2f& size);
    void setPosition(const Vec2f& position);

private:
    void rebuildPanels();

    std::vector<Panel> m_panels;
    Properties m_props;
    std::string m_name;

    bool m_dirty;
};

}  // namespace sl::ui
