#pragma once

#include <memory>

#include "fwd.h"
#include "Core.hpp"
#include "Memory.hpp"

namespace nova::core::detail {

int mainImpl(int argc, char** argv);

}  // namespace nova::core::detail

nova::core::UniqPtr<nova::core::Application> createApplication();

#define NOVA_ENTRYPOINT \
    int main(int argc, char** argv) { return nova::core::detail::mainImpl(argc, argv); }
