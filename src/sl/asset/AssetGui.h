#pragma once

#include "sl/gui/GuiApi.h"

namespace sl::asset {

struct AssetGui {
    virtual void onGui(gui::GuiApi&) = 0;
};

}