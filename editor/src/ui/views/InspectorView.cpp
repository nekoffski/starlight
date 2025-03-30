#include "InspectorView.hh"

namespace sle {

InspectorView::InspectorView(Widget::State& state) : Widget(state) {}

void InspectorView::render() {
    static auto defaultPanel = []() { sl::text("Nothing to show"); };
    std::invoke(getInspectorCallback().value_or(defaultPanel));
}

}  // namespace sle
