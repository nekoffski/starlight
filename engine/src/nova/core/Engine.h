#pragma once

#include <kc/sig/SignalHandler.h>

#include "fwd.h"

#include "nova/platform/Platform.h"

namespace nova::core {

class Engine : public kc::sig::SignalHandler {
   public:
    explicit Engine(const platform::Platform& platform);

    void run(Application& application);

   private:
    void loopStep(Application& application);
    void onSignal(int signal) override;

    bool m_shouldStop;

    platform::Platform m_platform;
};

}  // namespace nova::core