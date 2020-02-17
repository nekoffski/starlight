#pragma once

namespace starl::core::event {

struct Event {
    explicit Event(bool dispatchOnce = false) : dispatchOnce(dispatchOnce) {} 
    virtual ~Event() = default;

    const bool dispatchOnce;
    const bool isHandled{false};
};

}