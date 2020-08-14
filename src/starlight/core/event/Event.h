#pragma once

namespace sl::core::event {

struct Event {
    explicit Event(bool dispatchOnce = false)
        : dispatchOnce(dispatchOnce) {}
    virtual ~Event() = default;

    const bool dispatchOnce;
    const bool isHandled{ false };
};
}