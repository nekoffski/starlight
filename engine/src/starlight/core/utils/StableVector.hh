#pragma once

#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/core/memory/Memory.hh"

namespace sl {

template <typename T> class StableVector {
public:
    explicit StableVector(u64 size) : m_size(size), m_buffer(m_size, nullptr) {}

    template <typename C>
    requires Callable<C, void, T&>
    void forEach(C&& callback) {
        for (auto& slot : m_buffer)
            if (slot) callback(*slot);
    }

    template <typename... Args>
    requires std::is_constructible_v<T, Args...>
    T* emplace(Args&&... args) {
        for (auto& slot : m_buffer) {
            if (slot) {
                slot.emplace<T>(std::forward<Args>(args)...);
                return slot.get();
            }
        }
        return nullptr;
    }

    bool erase(T* value) {
        for (auto& slot : m_buffer) {
            if (slot && slot.get() == value) {
                slot.clear();
                return true;
            }
        }
        return false;
    }

    const u64 getCapacity() const { return m_size; }

private:
    const u64 m_size;
    std::vector<LocalPtr<T>> m_buffer;
};

}  // namespace sl
