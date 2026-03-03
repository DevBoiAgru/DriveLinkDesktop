#include "DriveLink.hpp"

int main() {

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 4;
    sf::RenderWindow window(
        sf::VideoMode({ dl::consts::IMAGE_W, dl::consts::IMAGE_H }), "DriveLink",
        sf::Style::None, sf::State::Windowed, settings
    );
    window.setFramerateLimit(dl::consts::FPS_LIMIT);

    sf::View view(
        sf::FloatRect(
            sf::Vector2f(0.f, 0.f), sf::Vector2f(float(dl::consts::IMAGE_W), float(dl::consts::IMAGE_H))
        )
    );
    window.setView(view);

    // Initialise fonts
    sf::Font fontRegular, fontBold;
    if (!fontRegular.openFromFile("assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Could not load font OpenSansRegular" << std::endl;
    }
    if (!fontBold.openFromFile("assets/fonts/OpenSans-SemiBold.ttf")) {
        std::cerr << "Could not load font OpenSansSemiBold" << std::endl;
    }

    // Initialise shaders
    if (!sf::Shader::isAvailable()) {
        std::cerr << "No shader support" << std::endl;
    }

    // Background of the app. Drawn using a fullscreen rect with a gradient shader
    sf::Shader bgGradientShader;
    if (!bgGradientShader.loadFromFile("assets/shaders/Gradient.frag", sf::Shader::Type::Fragment)) {
        std::cerr << "Could not load gradient shader." << std::endl;
    }
    bgGradientShader.setUniform("u_resolution", sf::Glsl::Vec2 { window.getSize() });
    sf::RectangleShape bgRect({ float(dl::consts::IMAGE_W), float(dl::consts::IMAGE_H) });
    bgRect.setFillColor({ 100, 40, 10 });

    // Title bar
    sf::RectangleShape titleBarShape({ float(dl::consts::IMAGE_W), dl::consts::TITLEBAR_HEIGHT });
    titleBarShape.setFillColor({ 0, 0, 0, 0 });
    sf::FloatRect titleBarArea = titleBarShape.getGlobalBounds();

    // UI List. Contains references to all the dl::ui::Components which are shown on screen
    std::vector<std::unique_ptr<dl::ui::Component>> uiComponents;

    // Close button
    sf::FloatRect closeBtnRect(
        { dl::consts::IMAGE_W - dl::consts::TITLEBAR_HEIGHT, 0 }, { dl::consts::TITLEBAR_HEIGHT, dl::consts::TITLEBAR_HEIGHT }
    );
    uiComponents.push_back(
        std::make_unique<dl::ui::Button>(
            closeBtnRect,
            sf::Color(0, 0, 0, 0),
            sf::Color(232, 17, 35, 255),
            sf::Color(146, 12, 38, 255),
            [&]() { window.close(); }
        )
    );
    uiComponents.push_back(
        std::make_unique<dl::ui::Cross>(
            25.f, 2, 3.f, sf::Vector2f(dl::consts::IMAGE_W - dl::consts::TITLEBAR_HEIGHT / 2, dl::consts::TITLEBAR_HEIGHT / 2), 45.f, sf::Color(255, 255, 255)
        )
    );
    // Minimise button
    sf::FloatRect minBtnRect(
        { dl::consts::IMAGE_W - 2 * dl::consts::TITLEBAR_HEIGHT, 0 }, { dl::consts::TITLEBAR_HEIGHT, dl::consts::TITLEBAR_HEIGHT }
    );
    uiComponents.push_back(
        std::make_unique<dl::ui::Button>(
            minBtnRect,
            sf::Color(0, 0, 0, 0),
            sf::Color(54, 54, 57, 255),
            sf::Color(74, 74, 77, 255),
            [&]() {
                dl::platform::minimiseWindow(window);
            }
        )
    );
    uiComponents.push_back(
        std::make_unique<dl::ui::Text>(
            "_", dl::consts::TITLEBAR_HEIGHT, fontBold,
            sf::Vector2f(dl::consts::IMAGE_W - 1.5 * dl::consts::TITLEBAR_HEIGHT, dl::consts::TITLEBAR_HEIGHT / 2)
        )
    );

    // Title bar app name
    uiComponents.push_back(
        std::make_unique<dl::ui::Text>(
            "DriveLink", dl::consts::TITLEBAR_HEIGHT / 2, fontRegular,
            sf::Vector2f(dl::consts::IMAGE_W / 2, dl::consts::TITLEBAR_HEIGHT / 2),
            dl::ui::TextAlign::AlignCenter
        )
    );

    // Input indicators
    sf::CircleShape connectionIndicatorCircle(5.f);
    connectionIndicatorCircle.setOrigin({ connectionIndicatorCircle.getRadius(), connectionIndicatorCircle.getRadius() });
    connectionIndicatorCircle.setPosition({ dl::consts::IMAGE_W / 2 + 57, dl::consts::TITLEBAR_HEIGHT / 2 + 2.5 }); // Eyeballed position
    connectionIndicatorCircle.setFillColor({ 255, 0, 0 });

    sf::Texture t_steering;
    if (!t_steering.loadFromFile("assets/UI_Steering.png")) {
        std::cerr << "Failed loading steering image" << std::endl;
    }
    t_steering.setSmooth(true);
    sf::Sprite steeringIndicator(t_steering);
    steeringIndicator.setScale({ 0.5, 0.5 });
    steeringIndicator.setOrigin({ steeringIndicator.getLocalBounds().size.x / 2, steeringIndicator.getLocalBounds().size.y / 2 });
    steeringIndicator.setPosition({ dl::consts::IMAGE_W * 0.75, (dl::consts::IMAGE_H + dl::consts::TITLEBAR_HEIGHT) / 2 });

    std::array<sf::RectangleShape, 3> pedalIndicatorsOuter;
    std::array<sf::RectangleShape, 3> pedalIndicatorsInner;
    float pedalIndicatorRectFullHeight = dl::consts::IMAGE_H * 0.5f;
    for (int i = 0; i < 3; i++) {
        sf::RectangleShape outerRect({ 70.f, pedalIndicatorRectFullHeight });
        outerRect.setOutlineThickness(1.f);
        outerRect.setFillColor({ 0, 0, 0, 0 });
        outerRect.setPosition(
            { dl::consts::IMAGE_W * 0.1f * (i + 1),
              dl::consts::IMAGE_H * 0.5f - pedalIndicatorRectFullHeight / 2 }
        );
        outerRect.setOutlineColor({ 192, 194, 171 });
        pedalIndicatorsOuter[i] = std::move(outerRect);

        sf::RectangleShape innerRect({ 70.f, 0.f });
        innerRect.setPosition(
            { dl::consts::IMAGE_W * 0.1f * (i + 1),
              dl::consts::IMAGE_H * 0.5f + pedalIndicatorRectFullHeight / 2 }
        );
        switch (i) {
        case 2:
            innerRect.setFillColor({ 0, 255, 0 });
            break;
        case 1:
            innerRect.setFillColor({ 255, 0, 0 });
            break;
        case 0:
            innerRect.setFillColor({ 0, 0, 255 });
            break;
        }
        pedalIndicatorsInner[i] = std::move(innerRect);
    }

    // Initialise input state and event listener server
    dl::InputState::GetInstance();
    static dl::network::InputListener inputListener(7001);
    inputListener.Start();

    dl::ui::UIState uiState;
    // App loop
    while (window.isOpen()) {
        // Process events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

#ifdef _DEBUG
            // Quit with Q while debugging
            if (event->is<sf::Event::KeyPressed>())
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Q))
                    window.close();
#endif

            if (const sf::Event::Resized* resized = event->getIf<sf::Event::Resized>()) {
                // Manage window size
                dl::enforceAspectRatio(window, resized->size.x, resized->size.y);

                // Update background gradient
                bgGradientShader.setUniform(
                    "u_resolution",
                    sf::Glsl::Vec2(window.getSize())
                );
            }
        }

        // Update UI State
        uiState.mousePosition = window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );
        uiState.isLeftClickDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        uiState.isRightClickDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
        uiState.isLeftClickPressed = uiState.isLeftClickDown && !uiState.previouslyLeftDown;
        uiState.isRightClickPressed = uiState.isRightClickDown && !uiState.previouslyRightDown;
        uiState.isLeftClickReleased = !uiState.isLeftClickDown && uiState.previouslyLeftDown;
        uiState.isRightClickReleased = !uiState.isRightClickDown && uiState.previouslyRightDown;
        window.clear();

        // std::cout << dl::InputState::GetInstance().getTimestamp() << std::endl;

        // If the last input event we recieved was older than 0.5 seconds, we are disconnected
        connectionIndicatorCircle.setFillColor(
            (
                (
                    std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
                    - dl::InputState::GetInstance().getTimestamp()
                )
                > 500
            )
                ? sf::Color({ 255, 0, 0 })
                : sf::Color({ 0, 255, 0 })
        );

        // Clockwise is positive, so we need a negative sign here
        steeringIndicator.setRotation(sf::radians(-dl::InputState::GetInstance().getSteering()));

        // Draw the background and the title bar
        window.draw(bgRect, &bgGradientShader);
        window.draw(titleBarShape);

        // Draw UI components and update the UI state
        for (auto& component : uiComponents) {
            component->render(window);

            component->update(uiState);
        }

        // Draw the raw SFML shapes
        window.draw(connectionIndicatorCircle);
        window.draw(steeringIndicator);
        for (int i = 0; i < 3; i++) {
            window.draw(pedalIndicatorsOuter[i]);

            float rectHeight = 0.f;
            sf::RectangleShape& indicatorInnerRect = pedalIndicatorsInner[i];
            sf::Vector2f rectPos = indicatorInnerRect.getPosition();
            float rectBottom = dl::consts::IMAGE_H * 0.5f + pedalIndicatorRectFullHeight / 2;

            switch (i) {
            case 2:
                rectHeight = dl::InputState::GetInstance().getThrottle() * pedalIndicatorRectFullHeight;
                indicatorInnerRect.setSize({ 70.f, rectHeight });
                indicatorInnerRect.setPosition({ rectPos.x, rectBottom - rectHeight });
                break;
            case 1:
                rectHeight = dl::InputState::GetInstance().getBrake() * pedalIndicatorRectFullHeight;
                indicatorInnerRect.setSize({ 70.f, rectHeight });
                indicatorInnerRect.setPosition({ rectPos.x, rectBottom - rectHeight });
                break;
            case 0:
                rectHeight = dl::InputState::GetInstance().getClutch() * pedalIndicatorRectFullHeight;
                indicatorInnerRect.setSize({ 70.f, rectHeight });
                indicatorInnerRect.setPosition({ rectPos.x, rectBottom - rectHeight });
                break;
            }

            window.draw(indicatorInnerRect);
        }

        window.display();
        uiState.previouslyLeftDown = uiState.isLeftClickDown;
        uiState.previouslyRightDown = uiState.isRightClickDown;
    }

    // Cleanup
    inputListener.Stop();
}