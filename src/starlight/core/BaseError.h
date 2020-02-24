#pragma once

#include <stdexcept>
#include <string>

namespace {
constexpr int ERROR_CODE_MULTIPLIER = 1000;
}

namespace starl::err {

class Exception : public std::exception {
public:
    explicit Exception(int category, int code)
        : m_category(category)
        , m_code(code) {
    }

    virtual std::string toStr() const = 0;

    int getGlobalCode() const {
        return m_category * ERROR_CODE_MULTIPLIER + m_code;
    }

    int getLocalCode() const {
        return m_code;
    }

protected:
    int m_code;
    int m_category;
};
}
