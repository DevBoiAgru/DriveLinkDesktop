#include "dl/ui/cross.hpp"

namespace dl {
namespace ui {

Cross::Cross(
    float armLength,
    int armCount,
    float thickness,
    sf::Vector2f position,
    float angle,
    sf::Color color
)
    : m_armCount(armCount)
    , m_armLength(armLength)
    , m_armThickness(thickness)
    , m_angle(angle)
    , m_position(position)
    , m_color(color) {

    m_arms.reserve(m_armCount);
    for (int i = 0; i < m_armCount; i++) {
        sf::RectangleShape rect({ m_armLength, m_armThickness });
        rect.setFillColor(m_color);
        rect.setOrigin({ m_armLength / 2, m_armThickness / 2 });
        rect.setRotation(sf::degrees(m_angle + (180.f / armCount) * i));
        rect.setPosition(m_position);
        m_arms.push_back(std::move(rect));
    }
}

void Cross::render(sf::RenderWindow& window) {
    for (int i = 0; i < m_armCount; i++) {
        window.draw(m_arms[i]);
    }
}

} // namespace ui
} // namespace dl