#include "DriveLink.hpp"

int main() {

    sf::RenderWindow window(
        sf::VideoMode({ dl::consts::IMAGE_W, dl::consts::IMAGE_H }), "DriveLink",
        sf::Style::None
    );

    sf::View view(
        sf::FloatRect(
            sf::Vector2f(0.f, 0.f), sf::Vector2f(float(dl::consts::IMAGE_W), float(dl::consts::IMAGE_H))
        )
    );
    window.setView(view);

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
    titleBarShape.setFillColor({ 255, 0, 0 });
    sf::FloatRect titleBarArea = titleBarShape.getGlobalBounds();

    // UI List. Contains references to all the dl::ui::Components which are shown on screen
    std::vector<std::unique_ptr<dl::ui::Component>> uiComponents;

    // Close button
    sf::FloatRect closeBtnRect({ 100.0, 100.0 }, { 100.0, 100.0 });
    uiComponents.push_back(
        std::make_unique<dl::ui::Button>(
            closeBtnRect, []() {
                std::cout << "Close button clocked";
            }
        )
    );

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
                dl::enforceAspectRatio(window, resized->size.x, resized->size.x);

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

        // Draw the background and the title bar
        window.draw(bgRect, &bgGradientShader);
        window.draw(titleBarShape);

        // Draw UI components and update the UI state
        for (auto& component : uiComponents) {
            component->render(window);

            component->update(uiState);
        }
        uiState.previouslyLeftDown = uiState.isLeftClickDown;
        uiState.previouslyRightDown = uiState.isRightClickDown;
    }

    // Cleanup
    inputListener.Stop();
}