#pragma once

#include <memory>

#include <xvent/EventListener.h>

#include "fwd.h"
#include "sl/graphics/Renderer.hpp"
#include "sl/gui/GuiApi.h"
#include "sl/scene/SceneSystems.hpp"

namespace sl::core {
class Input;
}

namespace sl::application {

class ApplicationContext : public xvent::EventListener {
    friend class Application;

public:
    explicit ApplicationContext(const std::string& ident)
        : xvent::EventListener(ident) {}

    virtual void onInit() = 0;
    virtual void onAttach() = 0;
    virtual void onDetach() = 0;

    virtual void handleInput(std::shared_ptr<core::Input>) = 0;
    virtual void update(std::shared_ptr<scene::SceneSystems> sceneSystems, float, float) = 0;
    virtual void render(std::shared_ptr<graphics::Renderer>) = 0;

    virtual void renderGui(gui::GuiApi&) {}
};
}

#define SL_CONTEXT using sl::application::ApplicationContext::ApplicationContext
