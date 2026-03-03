#include "dl/ui/button.hpp"

namespace dl {
namespace ui {

Button::Button(
    sf::Vector2f position,
    sf::Vector2f size,
    sf::Color normalColor, sf::Color hoveredColor, sf::Color activeColor,
    std::function<void()> onClick,
    std::function<void()> onRelease,
    std::function<void()> onPress,
    std::function<void()> onHoverStart,
    std::function<void()> onHoverEnd
)
    : Component()
    , m_position(position)
    , m_size(size)
    , m_normalColor(normalColor)
    , m_hoveredColor(hoveredColor)
    , m_activeColor(activeColor)
    , m_onClick(onClick)
    , m_onRelease(onRelease)
    , m_onPress(onPress)
    , m_onHoverStart(onHoverStart)
    , m_onHoverEnd(onHoverEnd) {
    m_area = sf::FloatRect(m_position, m_size);
    m_shape = sf::RectangleShape({ m_size.x, m_size.y });
    m_shape.setPosition(position);
    m_shape.setFillColor(m_normalColor);
}

// Helper constructors with less parameters
Button::Button(
    sf::Vector2f position,
    sf::Vector2f size,
    std::function<void()> onClick
)
    : Button(
          position,
          size,
          dl::consts::colors::buttonRegular,
          dl::consts::colors::buttonHover,
          dl::consts::colors::buttonActive,
          std::move(onClick),
          []() { },
          []() { },
          []() { },
          []() { }
      ) { }

Button::Button(
    sf::Vector2f position,
    sf::Vector2f size,
    sf::Color normal,
    sf::Color hovered,
    sf::Color active,
    std::function<void()> onClick
)
    : Button(
          position,
          size,
          normal,
          hovered,
          active,
          std::move(onClick),
          []() { },
          []() { },
          []() { },
          []() { }
      ) { }

Button::Button(
    sf::Vector2f position,
    sf::Vector2f size,
    sf::Color normal,
    sf::Color hovered,
    sf::Color active,
    std::function<void()> onPress,
    std::function<void()> onRelease
)
    : Button(
          position,
          size,
          normal,
          hovered,
          active,
          []() { },
          std::move(onRelease),
          std::move(onPress),
          []() { },
          []() { }
      ) { }

void Button::Button::update(const UIState& state) {
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
        m_onPress();
        updateColor();
    }

    if (m_active && state.isLeftClickReleased) {
        if (m_hovered) {
            m_onClick();
        }
        m_active = false;
        m_onRelease();
        updateColor();
    }

    m_prevHover = m_hovered;
}

void Button::Button::render(sf::RenderWindow& window) {
    window.draw(m_shape);
}

void Button::Button::updateColor() {
    if (m_active) {
        m_shape.setFillColor(m_activeColor);
    } else if (m_hovered) {
        m_shape.setFillColor(m_hoveredColor);
    } else {
        m_shape.setFillColor(m_normalColor);
    };
}

} // namespace ui
} // namespace dl