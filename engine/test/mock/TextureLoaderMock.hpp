#pragma once

#include "starlight/gfx/TextureLoader.h"

#include <gmock/gmock.h>

using namespace sl;
using namespace sl::gfx;

struct TextureLoaderMock : TextureLoader {
    MOCK_METHOD(
        core::UniqPtr<Texture>, load,
        (const std::string& name, const Texture::Properties& props, const void* pixels),
        (const, override)
    );

    MOCK_METHOD(core::UniqPtr<Texture>, load, (const std::string& name), (const, override));
};
