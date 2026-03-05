#pragma once

#include "dl/core/Constants.hpp"
#include "dl/core/Server.hpp"
#include "dl/platform/PlatformUtils.hpp"
#include "dl/resources/ResourceManager.hpp"
#include "dl/ui/button.hpp"
#include "dl/ui/cross.hpp"
#include "dl/ui/progress.hpp"
#include "dl/ui/text.hpp"

#include <memory>
#include <SFML/Graphics.hpp>

#ifdef _DEBUG
#include <iostream>
#endif

namespace dl {

class DriveLinkApp {
public:
    DriveLinkApp(ResourceManager& resources);
    ~DriveLinkApp() = default;

    int run();

private:
    void processEvent(const std::optional<sf::Event>& event);
    void render();
    void update();

    sf::RenderWindow m_window;
    ResourceManager& m_resources;
    dl::ui::UIState m_uiState;

    sf::Shader* m_bgShader {};
    bool m_shadersSupported = true;
    bool m_draggingTitlebar = false;
    sf::Vector2i m_dragOffset;

    std::unique_ptr<sf::Sprite> m_steeringSprite;

    std::vector<std::unique_ptr<dl::ui::Component>> m_uiComponents;
    sf::RectangleShape m_bgRect; // Background of the app
    sf::CircleShape m_connectionIndicator;
};

} // namespace dl