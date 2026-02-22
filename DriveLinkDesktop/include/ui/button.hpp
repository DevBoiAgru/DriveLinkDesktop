#pragma once

#include "component.hpp"

#include <functional>
#include <SFML/Graphics.hpp>

namespace dl {
namespace ui {

class Button : public Component {
public:
    Button(
        sf::FloatRect area, std::function<void()> onClick,
        sf::Color normalColor = { 100, 100, 100 }, sf::Color hoveredColor = { 220, 220, 50 }, sf::Color activeColor = { 50, 50, 50 }
    )
        : Component(area)
        , m_onClick(onClick)
        , m_normalColor(normalColor)
        , m_hoveredColor(hoveredColor)
        , m_activeColor(activeColor) {
    }

    void update(UIState& state) override {
        m_hovered = m_area.contains({ state.mousePosition.x, state.mousePosition.y });

        if (m_hovered && state.isLeftClickPressed) {
            m_active = true;
        }

        if (m_active && state.isLeftClickReleased) {
            if (m_hovered) {
                m_onClick();
            }
            m_active = false;
        }
    }

private:
    std::function<void()> m_onClick;
    sf::Color m_normalColor;
    sf::Color m_hoveredColor;
    sf::Color m_activeColor;
    bool m_active { false };
};

} // namespace ui
} // namespace dl