#pragma once

#include <dl/core/Constants.hpp>
#include <fstream>
#include <string_view>

namespace dl {
namespace utils {

enum LogLevel {
    Info,
    Warn,
    Error,
    SuperFatalDangerousError
};

class Logger {
public:
    ~Logger();

    static Logger& GetLogger();

    void logMessage(LogLevel lvl, std::string_view message);
    void flushLog();

    void info(std::string_view message);
    void warn(std::string_view message);
    void error(std::string_view message);
    void catastrophicError(std::string_view message);

private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream m_logFile;

    // Possible future improvement: Convert this to a zero blocking logger
    // using a writer thread, a message queue and mutex wait with a conditional
    // for checking if the queue is empty or not.
    // Currently good enough since the writes are not high frequency
};

} // namespace utils
} // namespace dl