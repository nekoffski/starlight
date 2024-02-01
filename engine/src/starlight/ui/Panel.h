#pragma once

#include <string>

#include "starlight/core/Core.h"
#include "starlight/core/math/Glm.h"

#include "Widget.h"

namespace sl::ui {

class Panel : public Widget {
public:
    struct Properties {
        Vec2f position;
        Vec2f size;
    };

    explicit Panel(
      const std::string& name, const Properties& props, Callback&& callback
    );
    void render() override;

    void setSize(const Vec2f& size);
    void setPosition(const Vec2f& position);

private:
    std::string m_name;
    Properties m_props;
    Callback m_callback;
};

class PanelCombo : public Widget {
public:
    using Properties = Panel::Properties;

    explicit PanelCombo(const std::string& name, const Properties& props);

    void render() override;

    PanelCombo& addPanel(const std::string& name, Callback&& callback);

    void setSize(const Vec2f& size);
    void setPosition(const Vec2f& position);

private:
    void rebuildPanels();

    std::vector<Panel> m_panels;
    Properties m_props;
    std::string m_name;
};

}  // namespace sl::ui
