#pragma once

namespace nova::core {

struct Application {
    virtual bool isRunning() const       = 0;
    virtual void update(float delatTime) = 0;
};

}  // namespace nova::core