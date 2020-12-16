#pragma once

#include <memory>

#include "fwd.h"
#include "sl/gui/GuiApi.h"
#include "sl/scene/SceneSystems.hpp"

namespace sl::platform::input {
class Input;
}

namespace sl::application {

class ApplicationContext {
    friend class Application;

public:
    virtual void onInit() = 0;
    virtual void onAttach() = 0;
    virtual void onDetach() = 0;

    virtual void handleInput(std::shared_ptr<platform::input::Input>) = 0;
    virtual void update(float, float) = 0;
    virtual void render(scene::SceneSystems& sceneSystems) = 0;

    virtual void renderGui(gui::GuiApi&) {}
};
}
