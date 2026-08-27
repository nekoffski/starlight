#pragma once

#include <exception>
#include <functional>

#include "Concepts.hh"
#include "Macros.hh"

namespace starlight::details {

template <typename T>
class Scoped : public NonCopyable {
   public:
    Scoped() = default;

    template <typename Callable>
        requires std::invocable<Callable, T&>
    Scoped(T resource, Callable&& destructor)
        : m_resource(resource),
          m_destructor(std::forward<Callable>(destructor)),
          m_empty(false) {}

    ~Scoped() {
        if (not m_empty) {
            m_destructor(m_resource);
        }
    }

    Scoped(Scoped&& oth)
        : m_resource(std::move(oth.m_resource)),
          m_destructor(std::move(oth.m_destructor)),
          m_empty(oth.m_empty) {
        oth.m_empty = true;
    }

    Scoped& operator=(Scoped&& oth) {
        if (this != &oth) {
            if (not m_empty) {
                m_destructor(m_resource);
            }
            m_resource = std::move(oth.m_resource);
            m_destructor = std::move(oth.m_destructor);
            m_empty = oth.m_empty;
            oth.m_empty = true;
        }
        return *this;
    }

    T& operator*() { return m_resource; }
    const T& operator*() const { return m_resource; }

    T& get() { return m_resource; }
    const T& get() const { return m_resource; }

    bool empty() const { return m_empty; }

   private:
    T m_resource;
    std::function<void(T&)> m_destructor;
    bool m_empty{true};
};

template <typename Callback, typename Condition>
class ScopeGuard {
   public:
    explicit ScopeGuard(Callback&& callback, Condition&& condition)
        : m_callback(std::forward<Callback>(callback)),
          m_condition(std::forward<Condition>(condition)) {}

    ~ScopeGuard() {
        if (m_condition()) {
            m_callback();
        }
    }

   private:
    Callback m_callback;
    Condition m_condition;
};

enum class _ScopeGuardOnExit {};

struct _ScopeGuardOnExitCondition {
    bool operator()() const { return true; }
};

template <typename Callback>
ScopeGuard<Callback, decltype(_ScopeGuardOnExitCondition{})> operator+(
    _ScopeGuardOnExit, Callback && callback
) {
    return ScopeGuard<Callback, decltype(_ScopeGuardOnExitCondition{})>(
        std::forward<Callback>(callback), _ScopeGuardOnExitCondition{}
    );
}

enum class _ScopeGuardOnFailure {};

struct _ScopeGuardOnFailureCondition {
    bool operator()() const {
        return exceptionsInFlight != std::uncaught_exceptions();
    }

    const int exceptionsInFlight = std::uncaught_exceptions();
};

template <typename Callback>
ScopeGuard<Callback, decltype(_ScopeGuardOnFailureCondition{})> operator+(
    _ScopeGuardOnFailure, Callback && callback
) {
    return ScopeGuard<Callback, decltype(_ScopeGuardOnFailureCondition{})>(
        std::forward<Callback>(callback), _ScopeGuardOnFailureCondition{}
    );
}

enum class _ScopeGuardOnSuccess {};

struct _ScopeGuardOnSuccessCondition {
    bool operator()() const {
        return exceptionsInFlight == std::uncaught_exceptions();
    }

    const int exceptionsInFlight = std::uncaught_exceptions();
};

template <typename Callback>
ScopeGuard<Callback, decltype(_ScopeGuardOnSuccessCondition{})> operator+(
    _ScopeGuardOnSuccess, Callback && callback
) {
    return ScopeGuard<Callback, decltype(_ScopeGuardOnSuccessCondition{})>(
        std::forward<Callback>(callback), _ScopeGuardOnSuccessCondition{}
    );
}

}  // namespace starlight::details

// clang-format off
#define ON_SCOPE_EXIT    auto ANONYMOUS_VAR(SCOPE_EXIT)    = starlight::details::_ScopeGuardOnExit      {} + [&]()
#define ON_SCOPE_FAIL    auto ANONYMOUS_VAR(SCOPE_FAIL)    = starlight::details::_ScopeGuardOnFailure   {} + [&]()
#define ON_SCOPE_SUCCESS auto ANONYMOUS_VAR(SCOPE_SUCCESS) = starlight::details::_ScopeGuardOnSuccess   {} + [&]()
// clang-format on
