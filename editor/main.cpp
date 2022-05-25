
#include <iostream>

#include "nova/core/Application.h"
#include "nova/core/Entrypoint.hpp"
#include "nova/core/Core.hpp"

#include "nova/event/Event.h"
#include "nova/event/Input.h"
#include "nova/platform/Keys.h"

class Editor : public nova::core::Application, public nova::event::EventObserver {
   public:
    explicit Editor() { nova::event::EventManager::get().registerObserver(this); }

    bool isRunning() const { return m_running; }

    void onEvent(nova::event::EventWrapper& event) override {
        if (event.is<nova::event::KeyEvent>()) {
            auto keyEvent = event.get<nova::event::KeyEvent>();

            if (keyEvent.key == NOVA_KEY_ESCAPE &&
                keyEvent.action == nova::core::KeyAction::press) {
                m_running = false;
            }
        }
    }

    void update(float deltaTime) {}

   private:
    bool m_running = true;
};

nova::core::UniqPtr<nova::core::Application> createApplication() {
    return nova::core::createUniqPtr<Editor>();
}

NOVA_ENTRYPOINT;
