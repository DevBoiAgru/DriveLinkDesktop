#include <dl/utility/Logger.hpp>

#include <ctime>
#include <cstring>

namespace dl {
namespace utils {

namespace {

// I hate these people what were they thinking
std::tm getLocalTime(std::time_t t) {
    std::tm tm {};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    return tm;
}
} // namespace

Logger::Logger() {
    m_logFile = std::ofstream(dl::consts::LOG_FILE_PATH);
    m_logFile.seekp(0);
}

Logger::~Logger() {
    m_logFile.flush();
    m_logFile.close();
}

Logger& Logger::GetLogger() {
    static Logger inst;
    return inst;
}

void Logger::logMessage(LogLevel lvl, std::string_view message) {
    char timestampBuf[64];
    std::string logLine;
    std::time_t now = std::time(nullptr);
    std::tm tm = getLocalTime(now);

    std::strftime(timestampBuf, sizeof(timestampBuf), "%Y-%m-%d %H:%M:%S", &tm);

    const char* logLevel;
    switch (lvl) {
    case Info:
        logLevel = "INFO";
        break;
    case Warn:
        logLevel = "WARN";
        break;
    case Error:
        logLevel = "ERROR";
        break;
    case SuperFatalDangerousError:
        logLevel = "CATASTROPHIC ERROR";
        break;
    default:
        logLevel = "UNKNOWN";
        break;
    }

    m_logFile.write("[", 1);
    m_logFile.write(timestampBuf, std::strlen(timestampBuf));
    m_logFile.write("] [", 3);
    m_logFile.write(logLevel, std::strlen(logLevel));
    m_logFile.write("] ", 2);
    m_logFile.write(message.data(), message.size());
    m_logFile.write("\n", 1);
}

void Logger::info(std::string_view message) {
    logMessage(dl::utils::LogLevel::Info, message);
}
void Logger::warn(std::string_view message) {
    logMessage(dl::utils::LogLevel::Warn, message);
    flushLog();
}
void Logger::error(std::string_view message) {
    logMessage(dl::utils::LogLevel::Error, message);
    flushLog();
}
void Logger::catastrophicError(std::string_view message) {
    logMessage(dl::utils::LogLevel::SuperFatalDangerousError, message);
    flushLog();
}

void Logger::flushLog() {
    m_logFile.flush();
}

} // namespace utils
} // namespace dl