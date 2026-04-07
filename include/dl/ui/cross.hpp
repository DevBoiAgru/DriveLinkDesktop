#pragma once

#include "component.hpp"

#include <functional>
#include <SFML/Graphics.hpp>

namespace dl {
namespace ui {

class Cross : public Component {
public:
    Cross(
        float armLength,
        int armCount,
        float thickness,
        sf::Vector2f position,
        float angle = 0.f,
        sf::Color color = { 255, 255, 255, 255 }
    );

    void render(sf::RenderWindow& window) override;

private:
    float m_armLength = 10.f;
    float m_armThickness = 2.f;
    float m_angle = 0.f;
    int m_armCount = 2;
    sf::Vector2f m_position;
    sf::Color m_color;
    std::vector<sf::RectangleShape> m_arms;
};

} // namespace ui
} // namespace dl