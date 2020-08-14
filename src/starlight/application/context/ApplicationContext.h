#pragma once

#include <memory>

#include "ApplicationContextResource.h"
#include "starlight/gui/GUIProxy.h"

namespace sl::platform::input {
class Input;
}

namespace sl::application::context {

class ApplicationContext {
public:
    ApplicationContext(ApplicationContextResource&& applicationContextResource)
        : m_applicationContextResource(applicationContextResource) {
    }

    virtual void onAttach() = 0;
    virtual void onDetach() = 0;

    virtual void handleInput(std::unique_ptr<platform::input::Input>&) = 0;
    virtual void update(float) = 0;
    virtual void render() = 0;

    virtual void renderGUI(std::shared_ptr<gui::GUIProxy>&) {}

protected:
    ApplicationContextResource m_applicationContextResource;
};
}