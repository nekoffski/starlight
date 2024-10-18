#pragma once

#include <vector>

#include "Component.hh"

namespace sl {

struct ComponentContainerBase {
    virtual ~ComponentContainerBase() = default;
};

template <typename T> class ComponentContainer : public ComponentContainerBase {
public:
    template <typename... Args> Component<T> add(u64 entityId, Args&&... args) {}

private:
    // std::vector<>
};

}  // namespace sl
