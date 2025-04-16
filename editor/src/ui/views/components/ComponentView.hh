#pragma once

#include "ui/Widget.hh"

#include <starlight/app/scene/Component.hh>

namespace sle {

struct ComponentView : Widget {
    using Widget::Widget;

    virtual ~ComponentView() = default;

    virtual void render(void* ptr) = 0;
};

template <typename T>
requires std::derived_from<T, sl::Component>
class ComponentViewBase : public ComponentView {
public:
    using ComponentView::ComponentView;
    using Type = T;

private:
    void render(void* ptr) override { render(*static_cast<T*>(ptr)); }
    virtual void render(T& component) = 0;
};

}  // namespace sle
