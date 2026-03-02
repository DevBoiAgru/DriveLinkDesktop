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

    void render(sf::RenderWindow& window) override {
        for (int i = 0; i < m_armCount; i++) {
            window.draw(m_arms[i]);
        }
    }

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