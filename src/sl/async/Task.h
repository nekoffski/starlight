#pragma once

#include <string>

namespace sl::async {

struct Task {
    class Handle {
    public:
        explicit Handle(Task* task);

        void disable();

    private:
        Task* m_task;
    };

    explicit Task();

    virtual bool shouldInvoke() = 0;
    virtual void invoke() = 0;

    virtual std::string getName() const = 0;

    bool isActive = true;

    std::string id;
};

}