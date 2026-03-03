#include "dl/ui/progress.hpp"

namespace dl {
namespace ui {

Progress::Progress(
    sf::Vector2f size,
    sf::Vector2f position,
    std::function<float()> fillGetter,
    sf::Color fillColor,
    bool horizontal,
    float outlineThickness,
    sf::Color outlineColor
)
    : m_outerRect(size)
    , m_innerRect(size)
    , m_fillGetter(fillGetter)
    , m_isHorizontal(horizontal) {

    m_outerRect.setPosition(position);
    m_outerRect.setFillColor(sf::Color::Transparent);
    m_outerRect.setOutlineThickness(outlineThickness);
    m_outerRect.setOutlineColor(outlineColor);

    m_innerRect.setPosition(position);
    m_innerRect.setFillColor(fillColor);

    sf::Vector2f rectSize = m_outerRect.getSize();

    m_outerRect.setOrigin(rectSize * 0.5f);
    m_innerRect.setOrigin(rectSize * 0.5f);
}

void Progress::update(const UIState& state) {
    float clamped = std::clamp(m_fillGetter(), 0.f, 1.f);

    sf::Vector2f outerSize = m_outerRect.getSize();
    sf::Vector2f pos = m_outerRect.getPosition();

    if (m_isHorizontal) {
        m_innerRect.setPosition(pos);
        m_innerRect.setSize({ outerSize.x * clamped, outerSize.y });
    } else {
        float filledHeight = outerSize.y * clamped;
        float yOffset = outerSize.y - filledHeight;

        m_innerRect.setPosition({ pos.x, pos.y + yOffset });
        m_innerRect.setSize({ outerSize.x, filledHeight });
    }
}

void Progress::render(sf::RenderWindow& window) {
    window.draw(m_outerRect);
    window.draw(m_innerRect);
}

} // namespace ui
} // namespace dl