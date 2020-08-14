#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

namespace {
constexpr int ERROR_CODE_MULTIPLIER = 1000;
}

namespace sl::err {

class Exception : public std::exception {
public:
    explicit Exception(int category, int code, std::string msg)
        : m_category(category)
        , m_code(code)
        , m_msg(std::move(msg)) {
    }

    virtual std::string toStr() const = 0;

    int getGlobalCode() const {
        return m_category * ERROR_CODE_MULTIPLIER + m_code;
    }

    int getLocalCode() const {
        return m_code;
    }

    const std::string& getMsg() const {
        return m_msg;
    }

    virtual const char* what() const noexcept override {
        return m_whatMsg.c_str();
    };

protected:
    int m_code;
    int m_category;
    std::string m_msg;
    std::string msg;

    std::string m_whatMsg;
};

inline std::ostream& operator<<(std::ostream& ss, const Exception& e) {
    ss << e.toStr();
    return ss;
}
}
