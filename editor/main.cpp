
#include <iostream>

#include "nova/core/Application.h"
#include "nova/core/Entrypoint.hpp"
#include "nova/core/Core.hpp"

class Editor : public nova::core::Application {};

nova::core::UniqPtr<nova::core::Application> createApplication() {
    return nova::core::createUniqPtr<Editor>();
}

NOVA_ENTRYPOINT;
