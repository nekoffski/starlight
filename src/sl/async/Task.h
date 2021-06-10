#pragma once

#include <functional>

#include "sl/core/Uuid.h"

namespace sl::async {

struct Task {
    class Handle {
    public:
        explicit Handle(Task* task)
            : m_task(task) {
        }

        void disable() {
            if (m_task != nullptr)
                m_task->isActive = false;
            m_task = nullptr;
        }

    private:
        Task* m_task;
    };

    explicit Task()
        : id(core::generateUuid()) {
    }

    virtual bool shouldInvoke() = 0;
    virtual void invoke() = 0;

    virtual std::string getName() const = 0;

    bool isActive = true;

    std::string id;
};

}