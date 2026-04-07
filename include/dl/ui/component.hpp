#pragma once

#include <dl/core/Constants.hpp>

#include <SFML/Graphics.hpp>

namespace dl {
namespace ui {

struct UIState {
    sf::Vector2f mousePosition;
    bool isMouseMoving;

    bool isLeftClickDown = false; // Down is state (Continous)
    bool isRightClickDown = false;

    bool isLeftClickPressed = false; // Pressed and Released are events (One frame only)
    bool isLeftClickReleased = false;

    bool isRightClickPressed = false;
    bool isRightClickReleased = false;

    bool previouslyLeftDown = false;
    bool previouslyRightDown = false;

    uint32_t buttons = 0;
};

class Component {
public:
    Component() { }

    virtual void render(sf::RenderWindow& window) { }

    virtual void update(const UIState& state) { };

    virtual ~Component() = default;
};

} // namespace ui
} // namespace dl