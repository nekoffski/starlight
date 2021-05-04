#pragma once

#include "sl/gui/GuiApi.h"

namespace sl::gui::assets {

struct AssetGuiProvider {
    virtual void render(GuiApi&) = 0;
};

}