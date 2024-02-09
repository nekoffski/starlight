#include "Core.h"

std::string resouceTypeToString(ResourceType type) {
    switch (type) {
        case ResourceType::mesh:
            return "Mesh";
        case ResourceType::texture:
            return "Texture";
        case ResourceType::shader:
            return "Shader";
    }
    return "Unknown";
}