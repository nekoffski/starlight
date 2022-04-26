#pragma once

#include <string>

namespace sl::async {

struct PeriodicTask {
    class Handle {
       public:
        Handle(PeriodicTask* task);

        void disable();

       private:
        PeriodicTask* m_task;
    };

    explicit PeriodicTask();

    virtual bool shouldInvoke() = 0;
    virtual void invoke()       = 0;

    virtual std::string getName() const = 0;

    bool isActive;

    std::string id;
};

}  // namespace sl::async