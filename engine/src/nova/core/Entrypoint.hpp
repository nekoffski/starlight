#pragma once

#include <memory>

#include "fwd.h"
#include "Core.hpp"

namespace nova::detail {

int mainImpl(int argc, char** argv);

}  // namespace nova::detail

nova::UniqPtr<nova::Application> createApplication();

#define NOVA_ENTRYPOINT \
    int main(int argc, char** argv) { return nova::detail::mainImpl(argc, argv); }
