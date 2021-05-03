#pragma once

#include "sl/gui/GuiApi.h"

namespace sl::gui::assets {

struct AssetGui {
    virtual void render(GuiApi&) = 0;
};

}