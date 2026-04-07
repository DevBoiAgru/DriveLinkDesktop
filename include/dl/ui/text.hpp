#pragma once

#include "component.hpp"
#include <dl/utility/Utils.hpp>

#include <functional>
#include <SFML/Graphics.hpp>

namespace dl {
namespace ui {

enum TextAlign {
    AlignNone = 0,
    AlignCenter,
    AlignLeft,
    AlignRight,
};

class Text : public Component {
public:
    Text(
        std::string text,
        int characterSize,
        sf::Font& font,
        sf::Vector2f position,
        TextAlign alignment = TextAlign::AlignCenter,
        sf::Color color = { 255, 255, 255, 255 }
    );

    void render(sf::RenderWindow& window) override;

private:
    std::string m_textContent;
    sf::Font& m_font;
    sf::Text m_text;
    TextAlign m_alignment = TextAlign::AlignCenter;
    int m_charSize;
    sf::Vector2f m_position;
    sf::Color m_color;
};

} // namespace ui
} // namespace dl