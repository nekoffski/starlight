#pragma once

namespace sl::core::sig {

class SignalHandler {
public:
    virtual void onSignal(int) = 0;
};
}
