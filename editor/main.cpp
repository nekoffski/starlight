
#include <iostream>

#include "starlight/core/Application.h"
#include "starlight/core/Entrypoint.hpp"
#include "starlight/core/Core.hpp"

#include "starlight/event/Event.h"
#include "starlight/event/Input.h"
#include "starlight/platform/Keys.h"

class Editor : public sl::core::Application, public sl::event::EventObserver {
   public:
    explicit Editor() { sl::event::EventManager::get().registerObserver(this); }

    bool isRunning() const { return m_running; }

    void onEvent(sl::event::EventWrapper& event) override {
        if (event.is<sl::event::KeyEvent>()) {
            auto keyEvent = event.get<sl::event::KeyEvent>();

            if (keyEvent.key == SL_KEY_ESCAPE && keyEvent.action == sl::core::KeyAction::press) {
                m_running = false;
            }
        }
    }

    void update(float deltaTime) {}

   private:
    bool m_running = true;
};

sl::core::UniqPtr<sl::core::Application> createApplication() {
    return sl::core::createUniqPtr<Editor>();
}

SL_ENTRYPOINT;
