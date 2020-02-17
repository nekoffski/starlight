#pragma once

#include <string>
#include <optional>

namespace starl::core::err {

using Category = int;
using Code = int;

enum class Severity : unsigned char { ERROR, WARNING, FATAL };

struct Error {
    explicit Error(Category cat, Code code, Severity sev, const std::string& msg="") 
        :   category(cat), code(code), severity(sev), msg(msg)
    {
    }

    operator bool() const {
        return code != 0;
    }

    Error& operator=(const Error& e) {
        const_cast<Category&>(category) = e.category;
        code = e.code;
        severity = e.severity;
        msg = e.msg;
        return *this;
    }

    const Category category;
    Code code;
    Severity severity;
    std::string msg;
};

const Error NO_ERROR = Error{-1, 0, Severity::WARNING};

}
