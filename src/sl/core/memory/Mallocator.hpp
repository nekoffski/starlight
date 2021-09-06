#pragma once

#include <cstdlib>
#include <iostream>
#include <limits>
#include <new>

#include <kc/core/Log.h>

template <typename T>
class Mallocator {
public:
    using ValueType = T;

    explicit Mallocator() = default;

    template <typename U>
    constexpr Mallocator(const Mallocator<U>&) noexcept {}

        // clang-format off
    [[nodiscard]] T* allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_alloc();

        if (auto p = static_cast<T*>(std::malloc(n * sizeof(T)))) {
            report(p, n);
            return p;
        }

        throw std::bad_alloc();
    }
    // clang-format on

    void deallocate(T* p, std::size_t n) noexcept {
        report(p, n, 0);
        std::free(p);
    }

private:
    void report(T* p, std::size_t n, bool alloc = true) const {
        LOG_INFO("{} {} bytes at {}", alloc ? "Alloc: " : "Dealloc: ", sizeof(T) * n, reinterpret_cast<void*>(p));
    }
};

template <class T, class U>
bool operator==(const Mallocator<T>&, const Mallocator<U>&) {
    return true;
}

template <class T, class U>
bool operator!=(const Mallocator<T>&, const Mallocator<U>&) {
    return false;
}
