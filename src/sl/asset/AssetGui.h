#pragma once



namespace sl::asset {

struct AssetGui {
    virtual void onGui(gui::GuiApi&) = 0;
};

}