
#include <iostream>

#include "nova/core/Application.h"
#include "nova/core/Entrypoint.hpp"
#include "nova/core/Core.hpp"

class Editor : public nova::Application {};

nova::UniqPtr<nova::Application> createApplication() { return nova::createUniqPtr<Editor>(); }

NOVA_ENTRYPOINT;
