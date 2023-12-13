#pragma once

#include <functional>

#include "RenderView.h"

namespace sl {

class UIRenderView : public RenderView {
public:
    using UICallback = std::function<void()>;

    explicit UIRenderView(UICallback&& callback) :
        m_uiCallback(std::move(callback)) {}

private:
    UICallback m_uiCallback;
};

}  // namespace sl