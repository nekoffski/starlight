#pragma once

#include <memory>

#include <starlight/platform/gui/impl/GUIImpl.h>

namespace starl::platform::gui {

class GUIAdapter {
public:
    GUIAdapter(misc::types::NotNullPtr<void> windowHandle)
        : m_pimpl(impl::GUIImpl::create(windowHandle)) {
    }

    void begin() {
        m_pimpl->begin();
    }

    void end() {
        m_pimpl->end();
    }

    void beginWindow(std::string title, math::Vec2 pos, math::Vec2 size) {
        m_pimpl->beginWindow(std::move(title), std::move(pos), std::move(size));
    }

    void endWindow() {
        m_pimpl->endWindow();
    }

private:
    std::unique_ptr<impl::GUIImpl> m_pimpl;
};
}