#pragma once

#include <SFML/Graphics.hpp>

namespace dl {
namespace ui {

struct UIState {
    sf::Vector2f mousePosition;
    bool isLeftClickDown = false; // Down is state (Continous)
    bool isRightClickDown = false;

    bool isLeftClickPressed = false; // Pressed and Released are events (One frame only)
    bool isLeftClickReleased = false;

    bool isRightClickPressed = false;
    bool isRightClickReleased = false;

    bool previouslyLeftDown = false;
    bool previouslyRightDown = false;
};

class Component {
public:
    Component() { }

    Component(sf::FloatRect area)
        : m_area(area) {
        m_shape = sf::RectangleShape({ area.size.x, area.size.y });
        m_shape.setPosition(area.position);
    }

    virtual void render(sf::RenderWindow& window) {
        window.draw(m_shape);
    }

    virtual void update(UIState& state) { };

    virtual ~Component() = default;

protected:
    bool m_hovered { false };
    bool m_pressed { false };
    sf::FloatRect m_area;
    sf::RectangleShape m_shape;
};

} // namespace ui
} // namespace dl