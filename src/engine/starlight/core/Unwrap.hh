#pragma once

#include "Log.hh"

namespace starlight {

template <typename T>
T unwrap(Result<T>&& result) {
    log::expect(result);
    return std::move(*result);
}

}  // namespace starlight
