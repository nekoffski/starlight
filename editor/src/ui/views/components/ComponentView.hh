#pragma once

#include "ui/Widget.hh"

namespace sle {

struct ComponentView : Widget {
    using Widget::Widget;

    virtual ~ComponentView() = default;

    virtual void render(void* ptr) = 0;
};

template <typename T> class ComponentViewBase : public ComponentView {
public:
    using ComponentView::ComponentView;

private:
    void render(void* ptr) override { render(*static_cast<T*>(ptr)); }
    virtual void render(T& component) = 0;
};

}  // namespace sle
