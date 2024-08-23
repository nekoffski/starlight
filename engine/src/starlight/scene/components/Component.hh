#pragma once

#include "starlight/core/Core.hh"
#include "starlight/core/Json.hh"

namespace sl {

struct Component {
    explicit Component(u64 id, u64 entityId);

    virtual kc::json::Node serialize() const = 0;

    const u64 id;
    const u64 entityId;
};

}  // namespace sl
