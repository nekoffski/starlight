#pragma once

#include <concepts>
#include <exception>
#include <sstream>
#include <string>
#include <type_traits>
#include <typeindex>

#include "ErrorCode.h"

namespace sl::core::error {

class Error : public std::exception {
public:
    explicit Error(ErrorCode errorCode, const std::string& details = "No details specified")
        : m_errorCode(errorCode)
        , m_details(details) {
    }

    // clang-format off
	template<typename T = ErrorCode>
 	T getErrorCode() const {
        return static_cast<T>(m_errorCode);
    }
    // clang-format on

    std::string toString() const {
        std::stringstream errorStream;
        errorStream << getName() << " - " << m_details << " - " << static_cast<int>(m_errorCode);

        return errorStream.str();
    }

    std::string getDetails() const {
        return m_details;
    }

    virtual std::string getName() const = 0;

    template <typename T>
    bool is() const {
        return getType() == std::type_index(typeid(T));
    }

private:
    ErrorCode m_errorCode;
    std::string m_details;

    virtual std::type_index getType() const = 0;
};
}

#define SL_DEFINE_SUB_ERROR(ClassName, BaseError)  \
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

#define SL_DEFINE_ERROR(ClassName) SL_DEFINE_SUB_ERROR(ClassName, Error)
