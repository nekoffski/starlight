#pragma once

#include <exception>
#include <functional>

#include "Macros.h"

namespace sl::core::detail {

template <typename Fun, typename Cond>
class ScopeGuard {
public:
    explicit ScopeGuard(Fun&& callback, Cond&& condition)
        : m_callback(std::move(callback))
        , m_condition(std::move(condition)) {
    }

    ~ScopeGuard() {
        if (m_condition())
            m_callback();
    }

private:
    Fun m_callback;
    Cond m_condition;
};

enum class _ScopeGuardOnExit {};

struct _ScopeGuardOnExitCond {
    bool operator()() const {
        return true;
    }
};

template <typename Fun>
ScopeGuard<Fun, decltype(_ScopeGuardOnExitCond {})> operator+(_ScopeGuardOnExit, Fun&& callback) {
    return ScopeGuard<Fun, decltype(_ScopeGuardOnExitCond {})>(
        std::forward<Fun>(callback), _ScopeGuardOnExitCond {});
}

enum class _ScopeGuardOnFailure {};

struct _ScopeGuardOnFailureCond {
    bool operator()() const {
        return uncaughtExceptionsAtCreation != std::uncaught_exceptions();
    }

    int uncaughtExceptionsAtCreation = std::uncaught_exceptions();
};

template <typename Fun>
ScopeGuard<Fun, decltype(_ScopeGuardOnFailureCond {})> operator+(_ScopeGuardOnFailure, Fun&& callback) {
    return ScopeGuard<Fun, decltype(_ScopeGuardOnFailureCond {})>(
        std::forward<Fun>(callback), _ScopeGuardOnFailureCond {});
}

enum class _ScopeGuardOnSuccess {};

struct _ScopeGuardOnSuccessCond {
    bool operator()() const {
        return uncaughtExceptionsAtCreation == std::uncaught_exceptions();
    }

    int uncaughtExceptionsAtCreation = std::uncaught_exceptions();
};

template <typename Fun>
ScopeGuard<Fun, decltype(_ScopeGuardOnSuccessCond {})> operator+(_ScopeGuardOnSuccess, Fun&& callback) {
    return ScopeGuard<Fun, decltype(_ScopeGuardOnSuccessCond {})>(
        std::forward<Fun>(callback), _ScopeGuardOnSuccessCond {});
}

}

// clang-format off
#define ON_SCOPE_EXIT    auto ANONYMOUS_VAR(SCOPE_EXIT)    = sl::core::detail::_ScopeGuardOnExit    {} + [&]()
#define ON_SCOPE_FAIL    auto ANONYMOUS_VAR(SCOPE_FAIL)    = sl::core::detail::_ScopeGuardOnFailure {} + [&]()
#define ON_SCOPE_SUCCESS auto ANONYMOUS_VAR(SCOPE_SUCCESS) = sl::core::detail::_ScopeGuardOnSuccess {} + [&]()
// clang-format on