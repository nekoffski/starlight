#pragma once

#include <unordered_map>

#include <starlight/ui/UI.hh>
#include <starlight/ui/widgets/Image.hh>

#include "ui/Widget.hh"

namespace sle {

class ResourcesView : public Widget {
public:
    explicit ResourcesView(Widget::State& state);

    void render();

private:
    // std::string m_nameBuffer;
};

}  // namespace sle
