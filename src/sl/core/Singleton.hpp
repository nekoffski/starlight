#pragma once

#include <memory>

#include "Logger.h"

namespace sl::core {

template <typename T>
class Singleton {
public:
    explicit Singleton() {
        SL_ASSERT(s_instance == nullptr, "Only 1 instance of singleton is allowed");
        s_instance = (T*)this;
    }

    static T* get() {
        return s_instance;
    }

    Singleton(const Singleton& oth) = delete;
    Singleton(Singleton&& oth) = delete;

    Singleton& operator=(const Singleton& oth) = delete;
    Singleton& operator=(Singleton&& oth) = delete;

private:
    inline static T* s_instance = nullptr;
};

}