#pragma once

#include <memory>

namespace sl::core {

class ITimestamp {
public:
    virtual float substract(const std::shared_ptr<ITimestamp>& rhs) = 0;
    virtual float value() = 0;
};

class IClockImpl {
public:
    virtual std::string getTimeString(std::string) = 0;
    virtual float getDeltaTime() = 0;
    virtual void update() = 0;
    virtual std::shared_ptr<ITimestamp> now() = 0;
};
}
