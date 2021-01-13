#pragma once

#include <memory>
#include <string>

namespace sl::core {

class Timestamp {
public:
    virtual float substract(std::shared_ptr<Timestamp> rhs) = 0;
    virtual float value() = 0;
};

class ClockImpl {
public:
    virtual std::string getTimeString(const std::string&) = 0;
    virtual float getDeltaTime() = 0;
    virtual void update() = 0;
    virtual std::shared_ptr<Timestamp> now() = 0;
};
}
