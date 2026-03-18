#include "dl/app/DriveLink.hpp"
#include "dl/utility/Logger.hpp"

namespace dl {

DriveLinkApp::DriveLinkApp(ResourceManager& resources)
    : m_resources(resources)
    , m_window(
          sf::VideoMode({ dl::consts::IMAGE_W, dl::consts::IMAGE_H }),
          "DriveLink",
          sf::Style::None,
          sf::State::Windowed,
          sf::ContextSettings(0, 0, 8) // Enable 8x anti aliasing
      )
    , m_bgRect({ float(dl::consts::IMAGE_W), float(dl::consts::IMAGE_H) })
    , m_connectionIndicator(5.f) {
    m_window.setFramerateLimit(dl::consts::UI_FPS_LIMIT);

    dl::utils::Logger& logger = dl::utils::Logger::GetLogger();

    if (!m_resources.loadFont("font_reg", "assets/fonts/OpenSans-Regular.ttf")) {
        logger.error("Failed to load regular font OpenSans-Regular.ttf from assets/ directory");
        throw std::runtime_error("Failed to load font - assets/fonts/OpenSans-Regular.ttf");
    }
    if (!m_resources.loadFont("font_bold", "assets/fonts/OpenSans-SemiBold.ttf")) {
        logger.error("Failed to load bold font OpenSans-SemiBold.ttf from assets/ directory");
        throw std::runtime_error("Failed to load font - assets/fonts/OpenSans-SemiBold.ttf");
    }
    if (!m_resources.loadTexture("ui_steering", "assets/textures/UI_Steering.png", true)) {
        logger.error("Failed to load image UI_Steering.png from assets/textures/ directory");
        throw std::runtime_error("Failed to load font - assets/textures/UI_Steering.png");
    }

    m_shadersSupported = sf::Shader::isAvailable();
    if (m_shadersSupported && !m_resources.loadShader("frag_bgGrad", sf::Shader::Type::Fragment, "assets/shaders/Gradient.frag")) {
        logger.warn("Failed to load shader frag_bgGrad, falling back to no shaders.");
        m_shadersSupported = false;
    }

    if (m_shadersSupported) {
        m_bgShader = &m_resources.getShader("frag_bgGrad");
    }

    // Start the input listener server
    static dl::network::InputListener inputListener(7001);
    inputListener.Start();

    // Create UI elements

    // ---- Titlebar Start ----
    m_uiComponents.push_back(
        std::make_unique<dl::ui::Button>(
            sf::Vector2f(0.f, 0.f), sf::Vector2f(float(dl::consts::IMAGE_W), dl::consts::TITLEBAR_HEIGHT),
            dl::consts::colors::Transparent,
            dl::consts::colors::Transparent,
            dl::consts::colors::Transparent,
            [&]() {
                m_draggingTitlebar = true;
                m_dragOffset = sf::Mouse::getPosition() - m_window.getPosition();
            },
            [&]() { m_draggingTitlebar = false; }
        )
    );
    // Close button
    m_uiComponents.push_back(
        std::make_unique<dl::ui::Button>(
            sf::Vector2f(float(dl::consts::IMAGE_W) - dl::consts::TITLEBAR_HEIGHT, 0.f),
            sf::Vector2f(dl::consts::TITLEBAR_HEIGHT, dl::consts::TITLEBAR_HEIGHT),
            dl::consts::colors::Transparent,
            dl::consts::colors::closeButtonHover,
            dl::consts::colors::closeButtonActive,
            [&]() { m_window.close(); }
        )
    );
    m_uiComponents.push_back(
        std::make_unique<dl::ui::Cross>(
            25.f, 2, 3.f, sf::Vector2f(dl::consts::IMAGE_W - dl::consts::TITLEBAR_HEIGHT / 2, dl::consts::TITLEBAR_HEIGHT / 2), 45.f, sf::Color(255, 255, 255)
        )
    );
    // Minimise button
    m_uiComponents.push_back(
        std::make_unique<dl::ui::Button>(
            sf::Vector2f(float(dl::consts::IMAGE_W) - 2 * dl::consts::TITLEBAR_HEIGHT, 0.f),
            sf::Vector2f(dl::consts::TITLEBAR_HEIGHT, dl::consts::TITLEBAR_HEIGHT),
            dl::consts::colors::Transparent,
            dl::consts::colors::buttonHover,
            dl::consts::colors::buttonActive,
            [&]() { dl::platform::minimiseWindow(m_window); }
        )
    );
    m_uiComponents.push_back(
        std::make_unique<dl::ui::Text>(
            "_", static_cast<int>(dl::consts::TITLEBAR_HEIGHT), m_resources.getFont("font_bold"),
            sf::Vector2f(dl::consts::IMAGE_W - 1.5 * dl::consts::TITLEBAR_HEIGHT, dl::consts::TITLEBAR_HEIGHT / 2),
            dl::ui::TextAlign::AlignCenter
        )
    );
    // Window Title
    m_uiComponents.push_back(
        std::make_unique<dl::ui::Text>(
            "DriveLink", static_cast<int>(dl::consts::TITLEBAR_HEIGHT / 2), m_resources.getFont("font_reg"),
            sf::Vector2f(dl::consts::IMAGE_W / 2, dl::consts::TITLEBAR_HEIGHT / 2),
            dl::ui::TextAlign::AlignCenter
        )
    );
    // ---- Titlebar End ----

    // Steering indicator
    m_steeringSprite = std::make_unique<sf::Sprite>(
        m_resources.getTexture("ui_steering")
    );

    auto steeringSpriteBounds = m_steeringSprite->getLocalBounds();
    m_steeringSprite->setScale({ 0.5, 0.5 });
    m_steeringSprite->setOrigin({ steeringSpriteBounds.size.x / 2, steeringSpriteBounds.size.y / 2 });
    m_steeringSprite->setPosition({ dl::consts::IMAGE_W * 0.75, (dl::consts::IMAGE_H + dl::consts::TITLEBAR_HEIGHT) / 2 });

    // Pedal indicators
    float pedalIndicatorHeight = dl::consts::IMAGE_H * 0.5f;
    float pedalIndicatorY = dl::consts::IMAGE_H * 0.5f;
    float pedalIndicatorStartX = dl::consts::IMAGE_W * 0.1f;
    auto& input = dl::InputState::GetInstance();

    m_uiComponents.push_back(
        std::make_unique<dl::ui::Progress>(
            sf::Vector2f(dl::consts::ui::PedalWidth, pedalIndicatorHeight),
            sf::Vector2f(pedalIndicatorStartX + 0 * (dl::consts::ui::PedalWidth + dl::consts::ui::PedalSpacing), pedalIndicatorY),
            [&]() { return input.getClutch(); }, dl::consts::colors::pedal1
        )
    );

    m_uiComponents.push_back(
        std::make_unique<dl::ui::Progress>(
            sf::Vector2f(dl::consts::ui::PedalWidth, pedalIndicatorHeight),
            sf::Vector2f(pedalIndicatorStartX + 1 * (dl::consts::ui::PedalWidth + dl::consts::ui::PedalSpacing), pedalIndicatorY),
            [&]() { return input.getBrake(); }, dl::consts::colors::pedal2
        )
    );

    m_uiComponents.push_back(
        std::make_unique<dl::ui::Progress>(
            sf::Vector2f(dl::consts::ui::PedalWidth, pedalIndicatorHeight),
            sf::Vector2f(pedalIndicatorStartX + 2 * (dl::consts::ui::PedalWidth + dl::consts::ui::PedalSpacing), pedalIndicatorY),
            [&]() { return input.getThrottle(); }, dl::consts::colors::pedal3
        )
    );

    // Button indicators
    float progressHeight = pedalIndicatorHeight;
    float spacing = dl::consts::ui::SpacingSmall;

    sf::Vector2f indicatorSize(50.f, 50.f);

    // Grid height
    float gridHeight = 5 * indicatorSize.y + 4 * spacing;

    // Align vertically with progress bars
    float startY = pedalIndicatorY - gridHeight * 0.5f;

    sf::Vector2f startPos(
        pedalIndicatorStartX + 3.f * (dl::consts::ui::PedalWidth + dl::consts::ui::PedalSpacing),
        startY
    );

    for (int i = 0; i < 8; i++) {
        int col = i % 2;
        int row = i / 2;

        sf::Vector2f pos(
            startPos.x + col * (indicatorSize.x + spacing),
            startPos.y + row * (indicatorSize.y + spacing)
        );

        m_uiComponents.push_back(
            std::make_unique<dl::ui::ButtonIndicator>(
                i,
                pos,
                indicatorSize,
                dl::consts::colors::Transparent,
                dl::consts::colors::inputButtonActive
            )
        );
    }

    m_connectionIndicator.setOrigin({ 5.f, 5.f });
    m_connectionIndicator.setPosition({ dl::consts::TITLEBAR_HEIGHT * 0.5f, dl::consts::TITLEBAR_HEIGHT * 0.5f });

    m_bgRect.setFillColor(dl::consts::colors::bgFallback);
    if (m_shadersSupported) {
        m_bgShader->setUniform("u_resolution", sf::Glsl::Vec2 { m_window.getSize() });
    }
}

int DriveLinkApp::run() {
    while (m_window.isOpen()) {
        while (const std::optional event = m_window.pollEvent()) {
            processEvent(event);
        }

        // Update UI State
        m_uiState.mousePosition = m_window.mapPixelToCoords(
            sf::Mouse::getPosition(m_window)
        );
        m_uiState.isLeftClickDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        m_uiState.isRightClickDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
        m_uiState.isLeftClickPressed = m_uiState.isLeftClickDown && !m_uiState.previouslyLeftDown;
        m_uiState.isRightClickPressed = m_uiState.isRightClickDown && !m_uiState.previouslyRightDown;
        m_uiState.isLeftClickReleased = !m_uiState.isLeftClickDown && m_uiState.previouslyLeftDown;
        m_uiState.isRightClickReleased = !m_uiState.isRightClickDown && m_uiState.previouslyRightDown;

        update();
        render();

        m_uiState.previouslyLeftDown = m_uiState.isLeftClickDown;
        m_uiState.previouslyRightDown = m_uiState.isRightClickDown;
    }

    return 0;
}

void DriveLinkApp::processEvent(const std::optional<sf::Event>& event) {
    if (event->is<sf::Event::Closed>())
        m_window.close();

    if (const sf::Event::MouseMoved* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
        m_uiState.isMouseMoving = true;
        if (m_draggingTitlebar) {
            m_window.setPosition(sf::Mouse::getPosition() - m_dragOffset);
        }
    } else {
        m_uiState.isMouseMoving = false;
    }

#ifdef _DEBUG
    // Quit with Q while debugging
    if (event->is<sf::Event::KeyPressed>())
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Q))
            m_window.close();
#endif
}

void DriveLinkApp::update() {
    dl::InputState& inputState = dl::InputState::GetInstance();

    m_steeringSprite->setRotation(sf::radians(-inputState.getSteering() * dl::consts::M_PI_2));
    m_uiState.buttons = inputState.getButtons();

    for (auto& component : m_uiComponents) {
        component->update(m_uiState);
    }

    // If the last input event we recieved was older than 2 seconds, we are disconnected
    m_connectionIndicator.setFillColor(
        (
            (
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
                - dl::InputState::GetInstance().getTimestamp()
            )
            > 2000
        )
            ? sf::Color({ 255, 0, 0 })
            : sf::Color({ 0, 255, 0 })
    );
}

void DriveLinkApp::render() {
    m_window.clear();

    if (m_shadersSupported) {
        m_window.draw(m_bgRect, m_bgShader);
    } else {
        m_window.draw(m_bgRect);
    }

    m_window.draw(m_connectionIndicator);
    m_window.draw(*m_steeringSprite);

    for (auto& component : m_uiComponents) {
        component->render(m_window);
    }

    m_window.display();
}

} // namespace dl
