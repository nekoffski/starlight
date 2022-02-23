#pragma once

#include <gmock/gmock.h>

#include "sl/gfx/Cubemap.h"

using namespace sl::gfx;

struct CubemapMock : Cubemap {
    struct Factory : Cubemap::Factory {
        MOCK_METHOD(std::unique_ptr<Cubemap>, create, (const CubemapFaces&));
        MOCK_METHOD(std::unique_ptr<Cubemap>, create, (unsigned int, unsigned int));
    };

    MOCK_METHOD(unsigned int, getBufferId, (), (const));
    MOCK_METHOD(void, bind, (unsigned int index));
    MOCK_METHOD(void, unbind, ());
};
