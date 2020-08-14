#include "GlfwTicker.h"

#include <GLFW/glfw3.h>

namespace sl::platform::clock::tickers {
float GlfwTicker::getTicks() {
    return glfwGetTime();
}
}