#pragma once

#include <SFML/Graphics.hpp>

namespace dl {
namespace ui {

struct UIState {
    sf::Vector2f mousePosition;
    bool isLeftClickDown; // Down is state
    bool isRightClickDown;

    bool isLeftClickPressed; // Pressed and Released are events (One frame only)
    bool isLeftClickReleased;

    bool isRightClickPressed;
    bool isRightClickReleased;

    bool previouslyLeftDown;
    bool previouslyRightDown;
};

class Component {
public:
    Component(sf::FloatRect area)
        : m_area(area) {
        m_shape = sf::RectangleShape({ area.size.x, area.size.y });
        m_shape.setPosition(area.position);
    }

    void render(sf::RenderWindow& window) {
        window.draw(m_shape);
    }

    virtual void update(UIState& state) {
        m_hovered = m_area.contains({ state.mousePosition.x, state.mousePosition.y });
    };

    virtual ~Component() = default;

protected:
    bool m_hovered { false };
    bool m_pressed { false };
    sf::FloatRect m_area;
    sf::RectangleShape m_shape;
};

} // namespace ui
} // namespace dl