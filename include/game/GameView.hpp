#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "game/GameConfig.hpp"
#include "game/GameModel.hpp"
#include "game/GameState.hpp"
#include "game/ui/Button.hpp"
#include "game/ui/Triangle.hpp"

namespace game {

enum class MenuAction { None, Start, Settings, Leaderboard, Exit };

enum class SettingsAction { None, FontPrev, FontNext, SizeUp, SizeDown };

enum class PauseAction { None, Resume, Menu };

class GameView {
public:
    GameView(int initialFontIndex, int initialCharacterSize);

    const sf::Font& fontAt(int index) const;
    std::size_t fontCount() const;

    void updateBackground(int delta);

    MenuAction hitTestMenu(sf::Vector2f point) const;
    SettingsAction hitTestSettings(sf::Vector2f point) const;
    bool hitTestReturn(sf::Vector2f point) const;
    PauseAction hitTestPause(sf::Vector2f point) const;

    void updateHover(sf::Vector2f mousePos);
    void applySettings(int fontIndex, int characterSize);

    void draw(sf::RenderWindow& window, const GameModel& model);

private:
    std::string buildLeaderboardText() const;

    std::vector<sf::Font> fonts_;
    std::vector<sf::Texture> backgrounds_;
    int backgroundIndex_;
    sf::Sprite backgroundSprite_;

    sf::Text title_;
    sf::Text rules_;

    ui::Button startButton_;
    ui::Button settingsButton_;
    ui::Button leadershipButton_;
    ui::Button exitButton_;
    std::vector<ui::Button*> menuButtons_;

    ui::Button returnButton_;
    ui::Button resumeButton_;
    ui::Button menuButton_;

    ui::Button exampleTextChange_;
    ui::Triangle left_;
    ui::Triangle right_;
    ui::Triangle up_;
    ui::Triangle down_;

    sf::Text gameOverText_;
    sf::Text scoreMsgText_;
    sf::Text continueText_;

    sf::Text scoreText_;
    sf::Text livesText_;
    sf::Text spawnText_;

    sf::Text leaderboardText_;
};

} // namespace game
