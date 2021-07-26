#pragma once

#include <memory>

#include <xvent/EventListener.h>

#include "SceneSystems.h"
#include "fwd.h"
#include "sl/core/WindowProxy.hpp"
#include "sl/core/fwd.h"
#include "sl/gfx/LowLevelRendererProxy.hpp"
#include "sl/gui/GuiApi.h"
#include "sl/gui/GuiApiProxy.hpp"

namespace sl::app {

class ApplicationContext : public xvent::EventListener {
    friend class Application;

public:
    explicit ApplicationContext(const std::string& ident)
        : xvent::EventListener(ident) {
    }

    virtual ~ApplicationContext() = default;

    virtual void onInit() = 0;
    virtual void onAttach() = 0;
    virtual void onDetach() = 0;

    virtual void update(SceneSystems& sceneSystems, float, float) = 0;
    virtual void render(gfx::LowLevelRenderer&) = 0;

    virtual void renderGui(gui::GuiApi&) { }

protected:
    std::shared_ptr<gui::GuiApiProxy> m_guiApiProxy;
    std::shared_ptr<gfx::LowLevelRendererProxy> m_lowLevelRendererProxy;
    std::shared_ptr<core::WindowProxy> m_windowProxy;
};
}
