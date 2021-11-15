#include <iostream>

#include "sl/async/AsyncManager.hpp"

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
    using namespace sl::async;

    AsyncManager m;

    AsyncManager::get().start();
    AsyncManager::get().callAsync<::Task>();

    AsyncManager::get().update(1.0f);

    AsyncManager::get().stop();

    std::cout << "Hi\n";
    return 0;
}
