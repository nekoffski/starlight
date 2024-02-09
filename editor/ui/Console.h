#pragma once

#include <string>

#include <fmt/core.h>

class Logger {
public:
    explicit Logger(std::string& buffer);

    template <typename... Args>
    void info(const std::string& format, Args&&... args) {
        writeLog("inf", fmt::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void debug(const std::string& format, Args&&... args) {
        writeLog("dbg", fmt::format(format, std::forward<Args>(args)...));
    }

private:
    void writeLog(const std::string& severity, const std::string& message);

    std::string& m_buffer;
};

class Console {
public:
    explicit Console();

    void render();
    void clear();
    Logger* getLogger();

private:
    std::string m_buffer;
    Logger m_logger;
};
