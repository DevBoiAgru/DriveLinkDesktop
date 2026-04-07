#pragma once
#include <SFML/Window.hpp>

namespace dl {
namespace platform {
enum MessageBoxType {
    Info,
    YesNo,
    Warning,
    Error
};

enum MessageBoxAction {
    Ok = 0,
    Cancel,
    Yes,
    No
};

void minimiseWindow(sf::Window& window);
MessageBoxAction showMessageBox(sf::Window* window, MessageBoxType type, sf::String title, sf::String message);
} // namespace platform
} // namespace dl