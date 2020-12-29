#pragma once

#include <memory>
#include <string_view>

namespace sl::core {

class Timestamp {
public:
    virtual float substract(std::shared_ptr<Timestamp> rhs) = 0;
    virtual float value() = 0;
};

class ClockImpl {
public:
    virtual std::string getTimeString(std::string_view) = 0;
    virtual float getDeltaTime() = 0;
    virtual void update() = 0;
    virtual std::shared_ptr<Timestamp> now() = 0;
};
}
