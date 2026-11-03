#pragma once

#include <kcore/Error.hh>

#include "Core.hh"
#include "ErrorCode.hh"

namespace sl {

using Error = kc::Error;

template <typename T>
using Result = kc::Result<T>;

}  // namespace sl
