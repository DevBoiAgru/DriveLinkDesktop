#pragma once

#include "component.hpp"

#include <functional>
#include <SFML/Graphics.hpp>

namespace dl {
namespace ui {

class Progress : public Component {
public:
    Progress(
        sf::Vector2f size,
        sf::Vector2f position,
        std::function<float()> fillGetter,
        sf::Color fillColor = dl::consts::colors::buttonActive,
        bool horizontal = false,
        float outlineThickness = 1.f,
        sf::Color outlineColor = dl::consts::colors::primaryAccent
    );

    void render(sf::RenderWindow& window) override;
    void update(const UIState& state) override;

private:
    sf::RectangleShape m_outerRect;
    sf::RectangleShape m_innerRect;
    std::function<float()> m_fillGetter;
    bool m_isHorizontal = false;
    float m_fillAmount = 0.f;
};

} // namespace ui
} // namespace dl