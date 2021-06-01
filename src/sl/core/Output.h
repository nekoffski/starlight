#pragma once

#include <memory>

namespace sl::core {

template <typename T>
struct Output {
    virtual void set(std::shared_ptr<T>& result) = 0;
};

}