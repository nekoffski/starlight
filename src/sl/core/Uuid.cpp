#include "Uuid.h"

#include <crossguid/guid.hpp>

namespace sl::core {

std::string generateUuid() {
    return xg::newGuid().str();
}

}