#include <dl/ui/buttonIndicator.hpp>
#include <bitset>
#include <iostream>
namespace dl {
namespace ui {

ButtonIndicator::ButtonIndicator(
    int buttonIndex,
    sf::Vector2f position,
    sf::Vector2f size,
    sf::Color normalColor,
    sf::Color activeColor,
    float outlineThickness,
    sf::Color outlineColor
)
    : Component()
    , m_btnIdx(buttonIndex)
    , m_position(position)
    , m_size(size)
    , m_normalColor(normalColor)
    , m_activeColor(activeColor) {

    m_area = sf::FloatRect(m_position, m_size);
    m_shape = sf::RectangleShape({ m_size.x, m_size.y });
    m_shape.setPosition(position);
    m_shape.setFillColor(m_normalColor);
    m_shape.setOutlineThickness(outlineThickness);
    m_shape.setOutlineColor(outlineColor);
}

void ButtonIndicator::render(sf::RenderWindow& window) {
    window.draw(m_shape);
}

void ButtonIndicator::update(const UIState& state) {
    if (state.buttons & (1 << m_btnIdx)) {
        m_shape.setFillColor(m_activeColor);
    } else {
        m_shape.setFillColor(m_normalColor);
    }
}

} // namespace ui
} // namespace dl