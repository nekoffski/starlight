#pragma once

#include <memory>

namespace sl::core {

template <typename T>
struct Output {
    virtual void set(std::unique_ptr<T> result) = 0;
};

}