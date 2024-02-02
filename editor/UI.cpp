#include "UI.h"

static constexpr float leftComboWidthFactor = 0.15f;

UI::UI(sl::u64 w, sl::u64 h, sl::RendererFrontend& renderer) :
    m_width(w), m_height(h),
    m_leftCombo(
      "left-combo",
      sl::ui::PanelCombo::Properties{
        .position             = {0,                         0},
        .size                 = { leftComboWidthFactor * w, h},
        .alignWithMainMenuBar = true,
}
    ) {
    m_mainMenu.addMenu("File")
      .addItem("Create", []() {})
      .addItem("Open", "Ctrl+O", []() {})
      .addItem("Save", "Ctrl+S", []() {})
      .addItem("Save as", []() {});
    m_mainMenu.addMenu("Help");

    m_leftCombo
      .addPanel(
        "Statistics",
        [&]() {
            const auto [renderedVertices, frameNumber, delta] =
              renderer.getFrameStatistics();

            sl::ui::text("Frame time: {}", delta);
            sl::ui::text("Frames per second: {}", int(1.0f / delta));
            sl::ui::text("Frame number: {}", frameNumber);
            sl::ui::separator();
            sl::ui::text("Rendered vertices: {}", renderedVertices);
            sl::ui::separator();
            sl::ui::text("8/9/0 - change render mode");
            sl::ui::text("6     - dump var logs");
            sl::ui::text("3/4   - switch camera");
            sl::ui::text("u     - on/off update");
        }
      )
      .addPanel("Scene", []() { sl::ui::text("Just a placeholder for now"); });
}

void UI::render() {
    m_mainMenu.render();
    m_leftCombo.render();
}
