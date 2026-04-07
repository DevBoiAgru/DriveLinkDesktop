#pragma once

#include <dl/core/Constants.hpp>
#include <dl/ui/component.hpp>

#include <functional>
#include <SFML/Graphics.hpp>

namespace dl {
namespace ui {

class Button : public Component {
public:
    Button(
        sf::Vector2f position,
        sf::Vector2f size,
        std::function<void()> onClick
    );

    Button(
        sf::Vector2f position,
        sf::Vector2f size,
        sf::Color normal,
        sf::Color hovered,
        sf::Color active,
        std::function<void()> onClick
    );

    Button(
        sf::Vector2f position,
        sf::Vector2f size,
        sf::Color normal,
        sf::Color hovered,
        sf::Color active,
        std::function<void()> onPress,
        std::function<void()> onRelease
    );

    Button(
        sf::Vector2f position,
        sf::Vector2f size,
        sf::Color normalColor,
        sf::Color hoveredColor,
        sf::Color activeColor,
        std::function<void()> onClick,
        std::function<void()> onRelease,
        std::function<void()> onPress,
        std::function<void()> onHoverStart,
        std::function<void()> onHoverEnd
    );

    void update(const UIState& state) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::FloatRect m_area;
    sf::RectangleShape m_shape;
    std::function<void()> m_onClick;
    std::function<void()> m_onRelease;
    std::function<void()> m_onPress;
    std::function<void()> m_onHoverStart;
    std::function<void()> m_onHoverEnd;
    bool m_prevHover { false };
    sf::Color m_normalColor;
    sf::Color m_hoveredColor;
    sf::Color m_activeColor;
    bool m_active { false };
    bool m_hovered { false };

    void updateColor();
};

} // namespace ui
} // namespace dl