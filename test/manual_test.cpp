#include <iostream>

#include "sl/async/AsyncEngine.hpp"
#include "sl/async/ThreadPool.hpp"

int main() {
    ASYNC_ENGINE().init();

    ASYNC_ENGINE().parallelLoop(247, [](const int i) {
        std::cout << "Call " << i << '\n';
    });

    ASYNC_ENGINE().deinit();

    std::cout << "Hi\n";
    return 0;
}
