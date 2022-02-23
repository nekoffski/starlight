#pragma once

#include <string>

namespace sl::async {

struct AsyncTask {
    virtual void executeAsync() = 0;

    virtual std::string asString() const { return "Not_specified"; }

    virtual void finalize() {}
};

}  // namespace sl::async