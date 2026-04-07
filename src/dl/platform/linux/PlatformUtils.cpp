#include <dl/platform/PlatformUtils.hpp>
#include <dl/utility/Logger.hpp>

namespace dl {
namespace platform {
void minimiseWindow(sf::Window& window) {
    dl::utils::Logger& logger = dl::utils::Logger::GetLogger();
    logger.error("Minimise not implemented for linux yet.");
}

MessageBoxAction showMessageBox(sf::Window* window, MessageBoxType type, sf::String title, sf::String message) {

dl::utils::Logger& logger = dl::utils::Logger::GetLogger();
    // Mapping the enum to a string for the log
    std::string typeStr;
    switch (type) {
        case Info:    typeStr = "INFO";    break;
        case Warning: typeStr = "WARNING"; break;
        case Error:   typeStr = "ERROR";   break;
        case YesNo:   typeStr = "YES/NO";  break;
        default:      typeStr = "UNKNOWN"; break;
    }

    // Logging the intended message box details
    logger.error("Message boxes not implemented for Linux yet.");
    logger.error("[" + typeStr + "] Title: " + title.toAnsiString());
    logger.error("Message: " + message.toAnsiString());

    // We return Ok as a default fallback
    return Ok;
}

} // namespace platform
} // namespace dl