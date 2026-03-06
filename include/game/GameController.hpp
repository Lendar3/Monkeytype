#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

#include "game/GameModel.hpp"
#include "game/GameView.hpp"

namespace game {

class GameController {
public:
    GameController();

    void run();

private:
    void processEvent(const sf::Event& event);
    void handleKeyPressed(const sf::Event::KeyPressed& keyEvent);
    void handleTextEntered(const sf::Event::TextEntered& textEvent);
    void handleMousePressed(const sf::Event::MouseButtonPressed& mouseEvent);
    void handleMouseMoved(const sf::Event::MouseMoved& moveEvent);

    void update();
    void render();

    sf::RenderWindow window_;
    GameModel model_;
    GameView view_;
    sf::Clock frameClock_;

    bool textEnteredSeen_ = false;
    std::optional<char> pendingFallbackChar_{};
};

} // namespace game
