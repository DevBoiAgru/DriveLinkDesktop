#include "dl/ui/text.hpp"

namespace dl {
namespace ui {

Text::Text(
    std::string text,
    int characterSize,
    sf::Font& font,
    sf::Vector2f position,
    TextAlign alignment,
    sf::Color color
)
    : m_textContent(text)
    , m_font(font)
    , m_text(m_font, m_textContent)
    , m_charSize(characterSize)
    , m_position(position)
    , m_alignment(alignment)
    , m_color(color) {
    m_text.setCharacterSize(m_charSize);
    m_text.setFillColor(m_color);

    sf::FloatRect textRect = m_text.getLocalBounds();

    switch (m_alignment) {
    case AlignCenter:
        m_text.setOrigin({ textRect.position.x + textRect.size.x / 2, textRect.position.y + textRect.size.y / 2 });
        break;
    case AlignLeft:
        m_text.setOrigin({ 0, textRect.position.y + textRect.size.y / 2 });
        break;
    case AlignRight:
        m_text.setOrigin({ textRect.position.x + textRect.size.x, textRect.position.y + textRect.size.y / 2 });
        break;
    case AlignNone:
        // Don't change anything
        break;
    }

    m_text.setPosition(m_position);

    // Round vectors to prevent weird aliasing on text
    m_text.setOrigin(dl::utils::roundVec(m_text.getOrigin()));
    m_text.setPosition(dl::utils::roundVec(m_text.getPosition()));
}

void Text::render(sf::RenderWindow& window) {
    window.draw(m_text);
}

} // namespace ui
} // namespace dl