#pragma once

#include "nova/gfx/TextureLoader.h"

#include <gmock/gmock.h>

using namespace nova;
using namespace nova::gfx;

struct TextureLoaderMock : TextureLoader {
    MOCK_METHOD(
        core::UniqPtr<Texture>, load,
        (const std::string& name, const Texture::Properties& props, const void* pixels),
        (const, override)
    );

    MOCK_METHOD(
        core::UniqPtr<Texture>, load, (const std::string& name, const std::string& path),
        (const, override)
    );
};
