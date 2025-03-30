#pragma once

#include <unordered_map>

#include <starlight/ui/UI.hh>
#include <starlight/ui/widgets/Image.hh>

#include "ui/Widget.hh"

namespace sle {

class InspectorView : public Widget {
public:
    explicit InspectorView(Widget::State& state);

    void render();
};

}  // namespace sle
