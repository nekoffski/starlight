#pragma once

#include <concepts>
#include <exception>
#include <sstream>
#include <string>

#include "Misc.hpp"

#include "ErrorCode.h"

namespace sl::core {

class Error : public std::exception {
public:
    explicit Error(ErrorCode errorCode, const std::string& details = "No details specified")
        : m_errorCode(errorCode)
        , m_details(details) {
    }

    template <typename T>
    T as() {
        return asImpl(std::type_identity<T>());
    }

    // clang-format off
	template<class T = ErrorCode>
 	T getErrorCode() const {
        return static_cast<T>(m_errorCode);
    }
    // clang-format on

    std::string getDetails() const {
        return m_details;
    }

    virtual std::string getName() const = 0;

    template <typename T>
    bool is() const {
        return getType() == typeIndex<T>();
    }

private:
    template <typename T>
    T asImpl(std::type_identity<T>) {
        return T(m_errorCode, m_details);
    }

    std::string asImpl(std::type_identity<std::string>) {
        return toString();
    }

    std::string toString() const {
        return getName() + " - " + m_details + " - " + std::to_string(static_cast<int>(m_errorCode));
    }

    ErrorCode m_errorCode;
    std::string m_details;

    virtual std::type_index getType() const = 0;
};
}

#define LOG_DEFINE_SUB_ERROR(ClassName, BaseError)  \
    class ClassName : public BaseError {           \
    public:                                        \
        using BaseError::BaseError;                \
                                                   \
        std::string getName() const override {     \
            return #ClassName;                     \
        }                                          \
                                                   \
    private:                                       \
        std::type_index getType() const override { \
            return typeid(ClassName);              \
        }                                          \
    };

#define LOG_DEFINE_SUB_ERROR_DF(ClassName, BaseError, DefErrorCode)              \
    class ClassName : public BaseError {                                        \
    public:                                                                     \
        explicit ClassName(const std::string& details = "No details specified", \
            ErrorCode errorCode = DefErrorCode)                                 \
            : BaseError(errorCode, details) {                                   \
        }                                                                       \
                                                                                \
        std::string getName() const override {                                  \
            return #ClassName;                                                  \
        }                                                                       \
                                                                                \
    private:                                                                    \
        std::type_index getType() const override {                              \
            return typeid(ClassName);                                           \
        }                                                                       \
    };

#define LOG_DEFINE_ERROR(ClassName) LOG_DEFINE_SUB_ERROR(ClassName, Error)
