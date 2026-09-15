#pragma once

#include <atomic>

#include "Core.hh"

namespace sl {

template <typename T>
class Ref {
    struct ControlBlock {
        template <typename... Args>
        explicit ControlBlock(Args&&... args)
            : data(std::forward<Args>(args)...) {}

        T data;
        std::atomic<i32> refCounter{1};
    };

   public:
    template <typename... Args>
        requires std::constructible_from<T, Args...>
    explicit Ref(Args&&... args)
        : m_controlBlock(new ControlBlock{std::forward<Args>(args)...}) {}

    ~Ref() {
        if (m_controlBlock && m_controlBlock->refCounter.fetch_sub(1) == 1) {
            delete m_controlBlock;
        }
    }

    Ref(const Ref& other) : m_controlBlock(other.m_controlBlock) {
        if (m_controlBlock) {
            m_controlBlock->refCounter.fetch_add(1);
        }
    }

    Ref(Ref&& other)
        : m_controlBlock(std::exchange(other.m_controlBlock, nullptr)) {}

    Ref& operator=(const Ref& other) = delete;
    Ref& operator=(Ref&& other) = delete;

   protected:
    T& data() { return m_controlBlock->data; }
    const T& data() const { return m_controlBlock->data; }

   private:
    ControlBlock* m_controlBlock;
};

}  // namespace sl
