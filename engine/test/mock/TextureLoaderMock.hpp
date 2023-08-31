#pragma once

#include "starlight/renderer/gpu/TextureLoader.h"

#include <gmock/gmock.h>

using namespace sl;
using namespace sl;

struct TextureLoaderMock : TextureLoader {
    MOCK_METHOD(
        UniqPtr<Texture>, load,
        (const std::string& name, const Texture::Properties& props, const void* pixels),
        (const, override)
    );
};
