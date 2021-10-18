#pragma once

#include <string>

namespace sl::gui {

void initGui(void* windowHandle);
void shutdownGui();

void addFont(const std::string& path, short unsigned min, short unsigned max);

void beginGui();
void endGui();

}