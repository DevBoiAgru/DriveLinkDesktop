#pragma once
#include <SFML/Graphics.hpp>

namespace dl {

namespace consts {

constexpr unsigned int IMAGE_W = 970;
constexpr unsigned int IMAGE_H = 560;
constexpr unsigned int UI_FPS_LIMIT = 30;
constexpr float aspect_ratio = float(IMAGE_W) / IMAGE_H;

constexpr float TITLEBAR_HEIGHT = 45;

namespace colors {

inline sf::Color Transparent(0, 0, 0, 0);
inline sf::Color bgFallback(20, 20, 20);
inline sf::Color titleBar(100, 10, 10);
inline sf::Color primaryAccent(192, 194, 171);

inline sf::Color pedal1(0, 0, 255);
inline sf::Color pedal2(255, 0, 0);
inline sf::Color pedal3(0, 255, 0);

inline sf::Color buttonRegular(94, 94, 97);
inline sf::Color buttonHover(54, 54, 57);
inline sf::Color buttonActive(74, 74, 77);

inline sf::Color closeButtonHover(232, 17, 35, 255);
inline sf::Color closeButtonActive(146, 12, 38, 255);

} // namespace colors

} // namespace consts
} // namespace dl