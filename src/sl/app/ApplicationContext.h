#pragma once

#include <memory>

#include <xvent/EventListener.h>

#include "fwd.h"
#include "sl/core/WindowProxy.hpp"
#include "sl/gfx/LowLevelRendererProxy.hpp"
#include "sl/gfx/Renderer.hpp"
#include "sl/gui/GuiApi.h"
#include "sl/gui/GuiApiProxy.hpp"
#include "sl/scene/SceneSystems.hpp"

namespace sl::core {
class Input;
}

namespace sl::app {

class ApplicationContext : public xvent::EventListener {
    friend class Application;

public:
    explicit ApplicationContext(const std::string& ident)
        : xvent::EventListener(ident) { }

    virtual void onInit() = 0;
    virtual void onAttach() = 0;
    virtual void onDetach() = 0;

    virtual void handleInput(core::Input&) = 0;
    virtual void update(scene::SceneSystems& sceneSystems, float, float) = 0;
    virtual void render(gfx::Renderer&) = 0;

    virtual void renderGui(gui::GuiApi&) { }

protected:
    std::shared_ptr<gui::GuiApiProxy> m_guiApiProxy;
    std::shared_ptr<gfx::LowLevelRendererProxy> m_lowLevelRendererProxy;
    std::shared_ptr<core::WindowProxy> m_windowProxy;
};
}
