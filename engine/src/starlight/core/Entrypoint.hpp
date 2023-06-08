#pragma once

#include <memory>

#include "fwd.h"
#include "Core.hpp"
#include "Memory.hpp"

namespace sl::core::detail {

int mainImpl(int argc, char** argv);

}  // namespace sl::core::detail

sl::core::UniqPtr<sl::core::Application> createApplication();

#define SL_ENTRYPOINT \
    int main(int argc, char** argv) { return sl::core::detail::mainImpl(argc, argv); }
