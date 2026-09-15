#pragma once

#include <future>

#include "Time.hh"

namespace sl {

template <typename T>
bool ready(std::future<T>& future) {
    return future.wait_for(0s) == std::future_status::ready;
}

}  // namespace sl