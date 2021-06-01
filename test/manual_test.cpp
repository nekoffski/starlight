#include <iostream>

#include "sl/async/AsyncEngine.hpp"
#include "sl/async/ThreadPool.hpp"

struct Task : sl::async::AsyncTask {
    void executeAsync() override {
        std::cout << "Async call: " << ++x << '\n';
    }

    void finalize() override {
        std::cout << "Finalize: " << ++x << '\n';
    }

    int x = 5;
};

int main() {
    ASYNC_ENGINE().init();
    ASYNC_ENGINE().executeAsyncTask<Task>();

    ASYNC_ENGINE().update(1.0f);

    ASYNC_ENGINE().deinit();

    std::cout << "Hi\n";
    return 0;
}
