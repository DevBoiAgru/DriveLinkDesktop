#pragma once
#include <SFML/Graphics.hpp>

namespace dl {

namespace consts {

// Program window constants
constexpr unsigned int IMAGE_W = 970;
constexpr unsigned int IMAGE_H = 560;
constexpr unsigned int UI_FPS_LIMIT = 30;
constexpr float aspect_ratio = float(IMAGE_W) / IMAGE_H;
constexpr float TITLEBAR_HEIGHT = 45;

// Log
const std::string LOG_FILE_PATH = "log.txt";

// SFML Colours for the entire app for elements
namespace colors {
inline sf::Color Transparent(0, 0, 0, 0);

inline sf::Color bgFallback(20, 20, 20);
inline sf::Color titleBar(100, 10, 10);
inline sf::Color primaryAccent(192, 194, 171);

inline sf::Color pedal1(77, 163, 255);
inline sf::Color pedal2(227, 59, 59);
inline sf::Color pedal3(76, 175, 80);
inline sf::Color inputButtonActive(176, 175, 180);

inline sf::Color buttonRegular(94, 94, 97);
inline sf::Color buttonHover(54, 54, 57);
inline sf::Color buttonActive(74, 74, 77);

inline sf::Color closeButtonHover(232, 17, 35, 255);
inline sf::Color closeButtonActive(146, 12, 38, 255);

} // namespace colors

namespace ui {
inline constexpr float PedalSpacing = 30.f;
inline constexpr float PedalWidth = 70.f;

inline constexpr float SpacingSmall = 8.f;
inline constexpr float SpacingMedium = 12.f;
inline constexpr float SpacingLarge = 32.f;
} // namespace ui

} // namespace consts
} // namespace dl