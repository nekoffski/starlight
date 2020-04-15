#pragma once

namespace starl::core::sig {

class SignalHandler {
public:
    virtual void onSignal(int) = 0;
};
}