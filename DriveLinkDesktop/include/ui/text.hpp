#pragma once

#include "component.hpp"
#include "utility/Utils.hpp"

#include <functional>
#include <SFML/Graphics.hpp>

namespace dl {
namespace ui {

typedef enum {
    AlignNone = 0,
    AlignCenter,
    AlignLeft,
    AlignRight,
} TextAlign;

class Text : public Component {
public:
    Text(
        std::string text,
        int characterSize,
        sf::Font& font,
        sf::Vector2f position,
        TextAlign alignment = TextAlign::AlignCenter,
        sf::Color color = { 255, 255, 255, 255 }
    )
        : m_textContent(text)
        , m_text(m_font, m_textContent)
        , m_font(font)
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
            m_text.setOrigin({ 0, textRect.position.y + textRect.size.y / 2 });
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

    void render(sf::RenderWindow& window) override {
        window.draw(m_text);
    }

private:
    std::string m_textContent;
    sf::Text m_text;
    sf::Font m_font;
    TextAlign m_alignment = TextAlign::AlignCenter;
    int m_charSize;
    sf::Vector2f m_position;
    sf::Color m_color;
};

} // namespace ui
} // namespace dl