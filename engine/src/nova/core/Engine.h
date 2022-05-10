#pragma once

#include <kc/sig/SignalHandler.h>

#include "fwd.h"

namespace nova::core {

class Engine : public kc::sig::SignalHandler {
   public:
    explicit Engine();

    void run(Application& application);

   private:
    void loopStep(Application& application);
    void onSignal(int signal) override;

    bool m_shouldStop;
};

}  // namespace nova::core