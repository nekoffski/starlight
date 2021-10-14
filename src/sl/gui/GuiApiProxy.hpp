// #pragma once

// #include <memory>

// #include "GuiApi.h"

// namespace sl::gui {

// class GuiApiProxy {
// public:
//     explicit GuiApiProxy(std::shared_ptr<GuiApi> guiApi)
//         : m_guiApi(guiApi) {
//     }

//     void addFont(const std::string& path, short unsigned min, short unsigned max) {
//         m_guiApi->addFont(path, min, max);
//     }

// private:
//     std::shared_ptr<GuiApi> m_guiApi;
// };

// }