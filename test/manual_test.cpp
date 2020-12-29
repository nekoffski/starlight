#include <iostream>

#include "sl/async/AsyncEngine.hpp"
#include "sl/async/ThreadPool.hpp"

int main() {
    sl::async::AsyncEngine::init();

    sl::async::AsyncEngine::parallelLoop(247, [](const int i) {
        std::cout << "Call " << i << '\n';
    });

    sl::async::AsyncEngine::deinit();

    std::cout << "Hi\n";
    return 0;
}
