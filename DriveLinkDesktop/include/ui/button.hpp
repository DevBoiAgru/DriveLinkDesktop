#pragma once

#include "component.hpp"

#include <functional>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace dl {
namespace ui {

class Button : public Component {
public:
    Button(
        sf::FloatRect area,
        sf::Color normalColor = { 250, 0, 0 }, sf::Color hoveredColor = { 0, 250, 0 }, sf::Color activeColor = { 0, 0, 250 },
        std::function<void()> onClick = []() { /* Does nothing by default */ },
        std::function<void()> onRelease = []() { /* Does nothing by default */ },
        std::function<void()> onHoverEnd = []() { /* Does nothing by default */ },
        std::function<void()> onHoverStart = []() { /* Does nothing by default */ }
    )
        : Component(area)
        , m_normalColor(normalColor)
        , m_hoveredColor(hoveredColor)
        , m_activeColor(activeColor)
        , m_onClick(onClick)
        , m_onRelease(onRelease)
        , m_onHoverStart(onHoverStart)
        , m_onHoverEnd(onHoverEnd) {
        m_shape.setFillColor(m_normalColor);
    }

    void update(UIState& state) override {
        m_hovered = m_area.contains({ state.mousePosition.x, state.mousePosition.y });

        // Hovering events
        if (m_hovered && !m_prevHover) {
            m_onHoverStart();
            updateColor();
        }

        if (!m_hovered && m_prevHover) {
            m_onHoverEnd();
            updateColor();
        }

        // Press and release events
        if (m_hovered && state.isLeftClickPressed) {
            m_active = true;
            updateColor();
        }

        if (m_active && state.isLeftClickReleased) {
            if (m_hovered) {
                m_onClick();
            }
            m_active = false;
            updateColor();
        }

        m_prevHover = m_hovered;
    }

private:
    std::function<void()> m_onClick;
    std::function<void()> m_onRelease;
    std::function<void()> m_onHoverStart;
    std::function<void()> m_onHoverEnd;
    bool m_prevHover { false };
    sf::Color m_normalColor;
    sf::Color m_hoveredColor;
    sf::Color m_activeColor;
    bool m_active { false };

    void updateColor() {
        if (m_active) {
            m_shape.setFillColor(m_activeColor);
        } else if (m_hovered) {
            m_shape.setFillColor(m_hoveredColor);
        } else {
            m_shape.setFillColor(m_normalColor);
        };
    }
};

} // namespace ui
} // namespace dl