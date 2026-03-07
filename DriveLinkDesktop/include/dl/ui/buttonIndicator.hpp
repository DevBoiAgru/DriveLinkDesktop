#pragma once

#include "dl/core/Constants.hpp"
#include "dl/ui/component.hpp"

#include <functional>
#include <SFML/Graphics.hpp>

namespace dl {
namespace ui {

class ButtonIndicator : public Component {
public:
    ButtonIndicator(
        int buttonIndex,
        sf::Vector2f position,
        sf::Vector2f size,
        sf::Color normal,
        sf::Color active,
        float outlineThickness = 1.f,
        sf::Color outlineColor = dl::consts::colors::primaryAccent
    );

    void update(const UIState& state) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::FloatRect m_area;
    sf::RectangleShape m_shape;
    int m_btnIdx;
    sf::Color m_normalColor;
    sf::Color m_activeColor;

    bool m_active { false };
};

} // namespace ui
} // namespace dl