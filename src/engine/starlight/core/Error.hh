#pragma once

#include <fmt/core.h>

#include <expected>
#include <optional>
#include <string>

#include "ErrorCode.hh"

namespace sl {

class Error {
   public:
    explicit Error(
        ErrorCode code, const std::string& message = "No details provided"
    );

    template <typename... Args>
    explicit Error(ErrorCode code, const std::string& fmt, Args&&... args)
        : m_code(code),
          m_message(
              fmt::format(fmt::runtime(fmt), std::forward<Args>(args)...)
          ) {}

    ErrorCode code() const;
    const std::string& message() const;

    template <typename... Args>
    static std::unexpected<Error> unexpected(
        ErrorCode code, const std::string& fmt, Args&&... args
    ) {
        return std::unexpected{Error{code, fmt, std::forward<Args>(args)...}};
    }

    static std::unexpected<Error> unexpected(const Error& error);

    static std::optional<Error> empty();

   private:
    ErrorCode m_code;
    std::string m_message;
};

template <typename T>
using Result = std::expected<T, Error>;

}  // namespace sl
