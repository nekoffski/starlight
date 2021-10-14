#pragma once

namespace sl::gui::assets {

struct AssetGuiProvider {
    virtual void render() = 0;
};

}